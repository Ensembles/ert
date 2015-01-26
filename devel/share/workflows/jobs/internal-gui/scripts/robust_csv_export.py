import os
import re
import pandas
from ert.enkf import ErtPlugin, CancelPluginException
from ert.enkf.export import SummaryCollector, GenKwCollector, MisfitCollector, DesignMatrixReader
from ert_gui.models.mixins.connectorless import DefaultPathModel, DefaultBooleanModel
from ert_gui.widgets.checkbox import CheckBox
from ert_gui.widgets.custom_dialog import CustomDialog
from ert_gui.widgets.list_edit_box import ListEditBox
from ert_gui.widgets.path_chooser import PathChooser



"""
Export of summary, misfit, design matrix data and gen kw into a single CSV file.

The script expects a single argument:

output_file: this is the path to the file to output the CSV data to

Optional arguments:

case_list: a comma separated list of cases to export (no spaces allowed) if no list is provided the current case is exported
design_matrix: a path to a file containing the design matrix
infer_iteration: If True the script will try to infer the iteration number by looking at the suffix of the case name (i.e. default_2 = iteration 2)
                 If False the script will use the ordering of the case list: the first item will be iteration 0, the second item will be iteration 1...
"""

class RobustCSVExportJob(ErtPlugin):
    INFER_HELP = ("<html>"
                 "If this is checked the iteration number will be inferred from the name i.e.:"
                 "<ul>"
                 "<li>case_name -> iteration: 0</li>"
                 "<li>case_name_0 -> iteration: 0</li>"
                 "<li>case_name_2 -> iteration: 2</li>"
                 "<li>case_0, case_2, case_5 -> iterations: 0, 2, 5</li>"
                 "</ul>"
                 "Leave this unchecked to set iteration number to the order of the listed cases:"
                 "<ul><li>case_0, case_2, case_5 -> iterations: 0, 1, 2</li></ul>"
                 "<br/>"
                 "</html>")

    def getName(self):
        return "Robust CSV Export"

    def getDescription(self):
        return "Export summary data, misfit, design matrix data and gen kw into a single CSV file."

    def inferIterationNumber(self, case_name):
        pattern = re.compile("_([0-9]+$)")
        match = pattern.search(case_name)

        if match is not None:
            return int(match.group(1))
        return 0


    def run(self, output_file, case_list=None, design_matrix_path=None, infer_iteration=True):

        cases = []
        if case_list is not None:
            cases = case_list.split(",")

        if case_list is None or len(cases) == 0:
            cases = [self.ert().getEnkfFsManager().getCurrentFileSystem().getCaseName()]


        if design_matrix_path is not None:
            if not os.path.exists(design_matrix_path):
                raise UserWarning("The design matrix file does not exists!")

            if not os.path.isfile(design_matrix_path):
                raise UserWarning("The design matrix is not a file!")


        data = pandas.DataFrame()

        for index, case in enumerate(cases):
            case = case.strip()

            if not self.ert().getEnkfFsManager().caseExists(case):
                raise UserWarning("The case '%s' does not exist!" % case)

            if not self.ert().getEnkfFsManager().caseHasData(case):
                raise UserWarning("The case '%s' does not have any data!" % case)

            if infer_iteration:
                iteration_number = self.inferIterationNumber(case)
            else:
                iteration_number = index

            summary_data = SummaryCollector.loadAllSummaryData(self.ert(), case)
            summary_data["ITERATION"] = iteration_number
            summary_data["CASE"] = case
            summary_data.set_index(["CASE", "ITERATION"], append=True, inplace=True)

            gen_kw_data = GenKwCollector.loadAllGenKwData(self.ert(), case)

            misfit_data = MisfitCollector.loadAllMisfitData(self.ert(), case)

            case_data = summary_data.join(gen_kw_data, how='inner')
            case_data = case_data.join(misfit_data, how='inner')

            if design_matrix_path is not None:
                design_matrix_data = DesignMatrixReader.loadDesignMatrix(design_matrix_path)
                case_data = case_data.join(design_matrix_data, how='inner')

            data = pandas.concat([data, case_data])

        data.to_csv(output_file)

        export_info = "Exported %d rows and %d columns to %s." % (len(data.index), len(data.columns), output_file)
        return export_info


    def getArguments(self, parent=None):
        description = "The CSV export requires some information before it starts:"
        dialog = CustomDialog("Robust CSV Export", description, parent)

        output_path_model = DefaultPathModel("output.csv")
        output_path_chooser = PathChooser(output_path_model, path_label="Output file path")

        design_matrix_default = ""
        data_kw = self.ert().getDataKW()
        if "DESIGN_MATRIX" in data_kw:
            design_matrix_default = data_kw[data_kw.indexForKey("DESIGN_MATRIX")][1]

        design_matrix_path_model = DefaultPathModel(design_matrix_default, is_required=False, must_exist=True)
        design_matrix_path_chooser = PathChooser(design_matrix_path_model, path_label="Design Matrix path")

        fs_manager = self.ert().getEnkfFsManager()
        all_case_list = fs_manager.getCaseList()
        all_case_list = [case for case in all_case_list if fs_manager.caseHasData(case)]
        list_edit = ListEditBox(all_case_list, "List of cases to export")

        infer_iteration_model = DefaultBooleanModel()
        infer_iteration_checkbox = CheckBox(infer_iteration_model, label="Infer iteration number", show_label=False)
        infer_iteration_checkbox.setToolTip(RobustCSVExportJob.INFER_HELP)

        dialog.addOption(output_path_chooser)
        dialog.addOption(design_matrix_path_chooser)
        dialog.addOption(list_edit)
        dialog.addOption(infer_iteration_checkbox)

        dialog.addButtons()

        success = dialog.showAndTell()

        if success:
            design_matrix_path = design_matrix_path_model.getPath()
            if design_matrix_path.strip() == "":
                design_matrix_path = None

            case_list = ",".join(list_edit.getItems())

            return [output_path_model.getPath(), case_list, design_matrix_path, infer_iteration_model.isTrue()]

        raise CancelPluginException("User cancelled!")


