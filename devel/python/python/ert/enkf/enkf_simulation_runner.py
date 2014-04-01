from ert.cwrap import CWrapper, BaseCClass
from ert.enkf import ENKF_LIB, EnkfFs
from ert.enkf.enums import EnkfInitModeEnum
from ert.util import BoolVector


class EnkfSimulationRunner(BaseCClass):

    def __init__(self, enkf_main):
        assert isinstance(enkf_main, BaseCClass)
        super(EnkfSimulationRunner, self).__init__(enkf_main.from_param(enkf_main).value, parent=enkf_main, is_reference=True)

    def runSimpleStep(self, active_realization_mask, initialization_mode, iter_nr):
        """ @rtype: bool """
        assert isinstance(active_realization_mask, BoolVector)
        assert isinstance(initialization_mode, EnkfInitModeEnum)
        return EnkfSimulationRunner.cNamespace().run_simple_step(self, active_realization_mask, initialization_mode , iter_nr)


    def runEnsembleExperiment(self, active_realization_mask):
        """ @rtype: bool """
        iter_nr = 0
        return self.runSimpleStep(active_realization_mask , EnkfInitModeEnum.INIT_CONDITIONAL , iter_nr)

    def runPostWorkflow(self):
        EnkfSimulationRunner.cNamespace().run_post_workflow(self)


    def smootherUpdate(self, target_fs):
        """ @rtype: bool """
        assert isinstance(target_fs, EnkfFs)
        return EnkfSimulationRunner.cNamespace().smoother_update(self, target_fs)



cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerType("enkf_simulation_runner", EnkfSimulationRunner)

EnkfSimulationRunner.cNamespace().run_assimilation  = cwrapper.prototype("void enkf_main_run_assimilation(enkf_simulation_runner, bool_vector, int, int, int)")
EnkfSimulationRunner.cNamespace().run_smoother      = cwrapper.prototype("void enkf_main_run_smoother(enkf_simulation_runner, char*, bool)")

EnkfSimulationRunner.cNamespace().run_simple_step   = cwrapper.prototype("bool enkf_main_run_simple_step(enkf_simulation_runner, bool_vector, enkf_init_mode_enum, int)")
EnkfSimulationRunner.cNamespace().smoother_update   = cwrapper.prototype("bool enkf_main_smoother_update(enkf_simulation_runner, enkf_fs)")
EnkfSimulationRunner.cNamespace().run_post_workflow = cwrapper.prototype("void enkf_main_run_post_workflow(enkf_simulation_runner)")
