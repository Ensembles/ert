from ert.enkf.enums.realization_state_enum import RealizationStateEnum
from ert_gui.models import ErtConnector
from ert_gui.models.mixins import ListModelMixin
from ert.enkf.state_map import StateMap


class CaseList(ErtConnector, ListModelMixin):

    def getList(self):
        fs = self.ert().getEnkfFsManager().getCurrentFileSystem()
        case_list = self.ert().getEnkfFsManager().getCaseList()
        return sorted(case_list)

    def addItem(self, value):
        # Creates a new filesystem. Value should be a case that does not exist
        enkf_fs = self.ert().getEnkfFsManager().getFileSystem(value)
        self.ert().getEnkfFsManager().switchFileSystem(enkf_fs)
        self.observable().notify(ListModelMixin.LIST_CHANGED_EVENT)


    def getAllCasesWithDataAndNotRunning(self):
        cases = self.getList()
        cases_with_data_and_not_running = []
        for case in cases:
            case_has_data = False
            case_not_running = False
            state_map = self.ert().getEnkfFsManager().getStateMapForCase(case)

            for state in state_map:
                if state == RealizationStateEnum.STATE_HAS_DATA:
                    case_has_data = True
                case_fs = self.ert().getEnkfFsManager().getFileSystem(case)
                if case_fs.writeCount() == 0:
                    case_not_running  = True

            if case_has_data and case_not_running:
                cases_with_data_and_not_running.append(case)

        return cases_with_data_and_not_running


    def getCaseRealizationStates(self, case_name):
        state_map = self.ert().getEnkfFsManager().getStateMapForCase(case_name)
        return [state for state in state_map]


    def externalModificationNotification(self):
        self.observable().notify(ListModelMixin.LIST_CHANGED_EVENT)







