from PyQt4.QtGui import QFormLayout, QLabel, QSpinBox

from ert_gui.ertwidgets import addHelpToWidget, AnalysisModuleSelector, CaseSelector
from ert_gui.ertwidgets.models.ertmodel import getRealizationCount, getRunPath, setNumberOfIterations, getNumberOfIterations
from ert_gui.ide.keywords.definitions import RangeStringArgument, ProperNameFormatArgument
from ert_gui.models.connectors.run import ActiveRealizationsModel, TargetCaseFormatModel
from ert_gui.simulation import SimulationConfigPanel
from ert_gui.simulation.models import IteratedEnsembleSmoother
from ert_gui.widgets.string_box import StringBox


class IteratedEnsembleSmootherPanel(SimulationConfigPanel):
    def __init__(self):
        SimulationConfigPanel.__init__(self, IteratedEnsembleSmoother())

        layout = QFormLayout()

        case_selector = CaseSelector()
        layout.addRow("Current case:", case_selector)

        run_path_label = QLabel("<b>%s</b>" % getRunPath())
        addHelpToWidget(run_path_label, "config/simulation/runpath")
        layout.addRow("Runpath:", run_path_label)

        number_of_realizations_label = QLabel("<b>%d</b>" % getRealizationCount())
        addHelpToWidget(number_of_realizations_label, "config/ensemble/num_realizations")
        layout.addRow(QLabel("Number of realizations:"), number_of_realizations_label)

        # The num_iterations_spinner does not track any external changes (will that ever happen?)
        num_iterations_spinner = QSpinBox()
        num_iterations_spinner.setMinimum(1)
        num_iterations_spinner.setMaximum(100)
        num_iterations_spinner.setValue(getNumberOfIterations())
        addHelpToWidget(num_iterations_spinner, "config/simulation/number_of_iterations")
        num_iterations_spinner.valueChanged[int].connect(setNumberOfIterations)

        layout.addRow("Number of iterations", num_iterations_spinner)

        iterated_target_case_format_model = TargetCaseFormatModel()
        self.iterated_target_case_format_field = StringBox(iterated_target_case_format_model, "Target case format", "config/simulation/iterated_target_case_format")
        self.iterated_target_case_format_field.setValidator(ProperNameFormatArgument())
        layout.addRow(self.iterated_target_case_format_field.getLabel(), self.iterated_target_case_format_field)


        self._analysis_module_selector = AnalysisModuleSelector(iterable=True, help_link="config/analysis/analysis_module")
        layout.addRow("Analysis Module:", self._analysis_module_selector)

        active_realizations_model = ActiveRealizationsModel()
        self.active_realizations_field = StringBox(active_realizations_model, "Active realizations", "config/simulation/active_realizations")
        self.active_realizations_field.setValidator(RangeStringArgument())
        layout.addRow(self.active_realizations_field.getLabel(), self.active_realizations_field)

        self.iterated_target_case_format_field.validationChanged.connect(self.simulationConfigurationChanged)
        self.active_realizations_field.validationChanged.connect(self.simulationConfigurationChanged)

        self.setLayout(layout)
        
    def isConfigurationValid(self):
        analysis_module = self._analysis_module_selector.getSelectedAnalysisModuleName()
        return self.iterated_target_case_format_field.isValid() and self.active_realizations_field.isValid() and analysis_module is not None


    def toggleAdvancedOptions(self, show_advanced):
        self.active_realizations_field.setVisible(show_advanced)
        self.layout().labelForField(self.active_realizations_field).setVisible(show_advanced)

        self._analysis_module_selector.setVisible(show_advanced)
        self.layout().labelForField(self._analysis_module_selector).setVisible(show_advanced)


    def getSimulationArguments(self):
        return {"analysis_module": self._analysis_module_selector.getSelectedAnalysisModuleName()}
