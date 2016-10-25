import time
from datetime import datetime

from ert.enkf import EnkfSimulationRunner
from ert.enkf.enums import HookRuntime
from ert_gui.shell import assertConfigLoaded, ErtShellCollection


class Simulations(ErtShellCollection):
    def __init__(self, parent):
        super(Simulations, self).__init__("simulations", parent)
        self.addShellFunction(name="settings",
                              function=Simulations.settings,
                              help_message="Show simulations settings.")

        self.addShellFunction(name="ensemble_experiment",
                              function=Simulations.ensembleExperiment,
                              help_message="Run Ensemble Experiment.")

    @assertConfigLoaded
    def settings(self, line):
        runpath = self.ert().getModelConfig().getRunpathAsString()

        iteration_count = self.ert().analysisConfig().getAnalysisIterConfig().getNumIterations()
        realizations = self.ert().getEnsembleSize()

        print("Runpath: %s" % runpath)
        print("Iteration count: %d" % iteration_count)
        print("Realization count: %d" % realizations)

    @assertConfigLoaded
    def ensembleExperiment(self, line):
        simulation_runner = EnkfSimulationRunner(self.ert())

        now = time.time()
        print("Ensemble Experiment started at: %s" % datetime.now().isoformat(sep=" "))

        print("Create the run path!")
        iteration_count = self.ert().analysisConfig().getAnalysisIterConfig().getNumIterations()
        active_realization_mask = BoolVector.createActiveMask("0-%d" % self.ert().getEnsembleSize())
        # Note that 0(zero) is used and not iteration_count
        simulation_runner.createRunPath(active_realization_mask, 0)

        print("Ensemble Experiment pre processing!")
        simulation_runner.runWorkflows(HookRuntime.PRE_SIMULATION)

        print("Start simulations!")
        num_successful_realizations = simulation_runner.runEnsembleExperiment()

        success = self.ert().analysisConfig().haveEnoughRealisations(num_successful_realizations, self.ert().getEnsembleSize())
        if not success:
            print("Error: Number of successful realizations is too low.\nYou can allow more failed realizations by setting / changing the MIN_REALIZATIONS configuration element!")
            return

        print("Ensemble Experiment post processing!")
        simulation_runner.runWorkflows(HookRuntime.POST_SIMULATION)

        print("Ensemble Experiment completed at: %s" % datetime.now().isoformat(sep=" "))

        diff = time.time() - now
        print("Running time: %d seconds" % int(diff))
