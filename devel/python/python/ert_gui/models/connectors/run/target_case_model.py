from ert_gui import ERT
from ert_gui.ertwidgets.models.ertmodel import getCurrentCaseName
from ert_gui.models import ErtConnector
from ert_gui.models.mixins import BasicModelMixin


class TargetCaseModel(ErtConnector, BasicModelMixin):

    def __init__(self):
        self.__target_case = self.getDefaultName()
        self.__custom = False
        ERT.ertChanged.connect(self.__caseChanged)
        super(TargetCaseModel, self).__init__()

    def getValue(self):
        """ @rtype: str """
        return self.__target_case

    def setValue(self, target_case):
        if target_case is None or target_case.strip() == "" or target_case == self.getDefaultName():
            self.__custom = False
            self.__target_case = self.getDefaultName()
        else:
            self.__custom = True
            self.__target_case = target_case

        self.observable().notify(self.VALUE_CHANGED_EVENT)


    def getDefaultName(self):
        case_name = getCurrentCaseName()
        return "%s_smoother_update" % case_name

    def __caseChanged(self):
        if not self.__custom:
            self.__target_case = self.getDefaultName()
            self.observable().notify(self.VALUE_CHANGED_EVENT)