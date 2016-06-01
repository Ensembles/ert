#  Copyright (C) 2016 Statoil ASA, Norway.
#
#  This file is part of ERT - Ensemble based Reservoir Tool.
#
#  ERT is free software: you can redistribute it and/or modify it under the
#  terms of the GNU General Public License as published by the Free Software
#  Foundation, either version 3 of the License, or (at your option) any later
#  version.
#
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
#  A PARTICULAR PURPOSE.
#
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
#  for more details.
from ert.enkf.enums import EnkfInitModeEnum
from ert.enkf.enums import HookRuntime

from ert_gui.models.connectors.run import ActiveRealizationsModel,\
    TargetCaseFormatModel, AnalysisModuleModel, BaseRunModel
from ert_gui.models.mixins import ErtRunError

from ert.util import BoolVector


class MultipleDataAssimilation(BaseRunModel):
    """
    Run Multiple Data Assimilation (MDA) Ensemble Smoother with custom weights.
    """

    def __init__(self):
        super(MultipleDataAssimilation, self).__init__(name="Multiple Data Assimilation", phase_count=2)
        self.weights = "1" # default value

    def getWeights(self):
        return self.weights

    def setWeights(self, weights):
        self.weights = weights

    def setAnalysisModule(self):
        module_name = AnalysisModuleModel().getCurrentChoice()
        module_load_success = self.ert().analysisConfig().selectModule(module_name)

        if not module_load_success:
            raise ErtRunError("Unable to load analysis module '%s'!" % module_name)


    def runSimulations(self):
        weights = self.parseWeights(self.weights)
        iteration_count = len(weights)

        print("Running MDA ES for %s  iterations\t%s" % (iteration_count, ", ".join(str(weight) for weight in weights)))
        weights = self.normalizeWeights(weights)

        weight_string = ", ".join(str(round(weight,3)) for weight in weights)
        print("Running MDA ES on (weights normalized)\t%s" % weight_string)


        self.setPhaseCount(iteration_count+2) # pre + post + weights

        target_case_format = TargetCaseFormatModel()

        source_fs = self.ert().getEnkfFsManager().getCurrentFileSystem()
        target_case_name = target_case_format.getValue() % 0
        target_fs = self.ert().getEnkfFsManager().getFileSystem(target_case_name)

        if not source_fs == target_fs:
            self.ert().getEnkfFsManager().switchFileSystem(target_fs)
            self.ert().getEnkfFsManager().initializeCurrentCaseFromExisting(source_fs, 0)

        active_realization_mask = BoolVector(True, self.ert().getEnsembleSize())


        phase_string = "Running MDA ES %d iteration%s." % (iteration_count, ('s' if (iteration_count != 1) else ''))
        self.setPhaseName(phase_string, indeterminate=True)

        self.ert().getEnkfSimulationRunner().createRunPath(active_realization_mask, 1)
        self.ert().getEnkfSimulationRunner().runWorkflows( HookRuntime.PRE_SIMULATION )


        for iteration, weight in enumerate(weights):
            self.simulateAndPostProcess(target_case_format, active_realization_mask, iteration)

            # We exit because the user has pressed 'Kill all simulations'.
            if self.userExitCalled( ):
                self.setPhase(iteration_count + 2, "Simulations stopped")
                return

            # We exit because there are too few realisations left for updating.
            if not self.checkSuccessCount( active_realization_mask ):
                self.setPhase(iteration_count + 2, "Simulations failed")
                return

            self.update(target_case_format, iteration, weights[iteration])


        self.setPhaseName("Post processing...", indeterminate=True)
        self.simulateAndPostProcess(target_case_format, active_realization_mask, iteration_count)

        self.setPhase(iteration_count + 2, "Simulations completed.")


    def update(self, target_case_format, iteration, weight):
        source_fs = self.ert().getEnkfFsManager().getCurrentFileSystem()
        next_iteration = (iteration + 1)
        next_target_case_name = target_case_format.getValue() % next_iteration
        target_fs = self.ert().getEnkfFsManager().getFileSystem(next_target_case_name)

        phase_string = "Analyzing iteration: %d with weight %f" % (next_iteration, weight)
        self.setPhase(self.currentPhase() + 1, phase_string, indeterminate=True)
        self.ert().analysisConfig().setGlobalStdScaling(weight)
        success = self.ert().getEnkfSimulationRunner().smootherUpdate(source_fs, target_fs)

        if not success:
            raise UserWarning("Analysis of simulation failed for iteration: %d!" % next_iteration)


    def simulateAndPostProcess(self, target_case_format, active_realization_mask, iteration):
        target_case_name = target_case_format.getValue() % iteration

        target_fs = self.ert().getEnkfFsManager().getFileSystem(target_case_name)
        self.ert().getEnkfFsManager().switchFileSystem(target_fs)

        phase_string = "Running simulation for iteration: %d" % iteration
        self.setPhaseName(phase_string, indeterminate=True)
        self.ert().getEnkfSimulationRunner().createRunPath(active_realization_mask, iteration)

        phase_string = "Pre processing for iteration: %d" % iteration
        self.setPhaseName(phase_string)
        self.ert().getEnkfSimulationRunner().runWorkflows( HookRuntime.PRE_SIMULATION )


        phase_string = "Running forecast for iteration: %d" % iteration
        self.setPhaseName(phase_string, indeterminate=False)
        success = self.ert().getEnkfSimulationRunner().runSimpleStep(active_realization_mask, EnkfInitModeEnum.INIT_CONDITIONAL, iteration)
        
        phase_string = "Post processing for iteration: %d" % iteration
        self.setPhaseName(phase_string, indeterminate=True)
        self.ert().getEnkfSimulationRunner().runWorkflows(HookRuntime.POST_SIMULATION)

        return success
    

    # This is completely broken; the success_count will only count the
    # number of realisations in the mask which was sent in to the
    # runSimulations( ) method.
    def checkSuccessCount(self, active_realization_mask):
        min_realization_count = self.ert().analysisConfig().getMinRealisations()
        success_count = active_realization_mask.count()

        if success_count < min_realization_count:
            return False
        return True

    
    def normalizeWeights(self, weights):
        """ :rtype: list of float """
        if not weights:
            return []
        from math import sqrt
        length = sqrt(sum((1.0 / x) * (1.0 / x) for x in weights))
        return [x * length for x in weights]


    def parseWeights(self, weights):
        if not weights:
            return []
        elements = weights.split(",")
        result = []
        for element in elements:
            element = element.strip()
            try:
                f = float(element)
                if f == 0:
                    print 'Warning: 0 weight, will ignore'
                else:
                    result.append(f)
            except ValueError:
                print 'Warning: cannot parse weight %s' % element

        return result
