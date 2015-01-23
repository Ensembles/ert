import math
from pandas import DataFrame, MultiIndex
import numpy
from ert.enkf import ErtImplType, EnKFMain, EnkfFs, RealizationStateEnum, GenKwConfig
from ert.enkf.plot_data import EnsemblePlotGenKW
from ert.util import BoolVector


class GenKwCollector(object):

    @staticmethod
    def createActiveList(ert, fs):
        state_map = fs.getStateMap()
        ens_mask = BoolVector(False, ert.getEnsembleSize())
        state_map.selectMatching(ens_mask, RealizationStateEnum.STATE_HAS_DATA)
        active_list = BoolVector.createActiveList(ens_mask)

        return [iens for iens in active_list]

    @staticmethod
    def getAllGenKwKeys(ert):
        """ @rtype: list of str """
        gen_kw_keys = ert.ensembleConfig().getKeylistFromImplType(ErtImplType.GEN_KW)
        gen_kw_keys = sorted([key for key in gen_kw_keys], key=lambda k : k.lower())

        gen_kw_list = []
        for key in gen_kw_keys:
            enkf_config_node = ert.ensembleConfig().getNode(key)
            gen_kw_config = enkf_config_node.getModelConfig()
            assert isinstance(gen_kw_config, GenKwConfig)

            for keyword_index, keyword in enumerate(gen_kw_config):
                gen_kw_list.append("%s:%s" % (key, keyword))

                if gen_kw_config.shouldUseLogScale(keyword_index):
                    gen_kw_list.append("LOG10_%s:%s" % (key, keyword))

        return gen_kw_list

    @staticmethod
    def loadAllGenKwData(ert, case_name):
        """
        @type ert: EnKFMain
        @type case_name: str
        @rtype: DataFrame
        """
        fs = ert.getEnkfFsManager().getFileSystem(case_name)

        realizations = GenKwCollector.createActiveList(ert, fs)
        gen_kw_keys = GenKwCollector.getAllGenKwKeys(ert)

        gen_kw_array = numpy.empty(shape=(len(gen_kw_keys), len(realizations)), dtype=numpy.float64)
        gen_kw_array.fill(numpy.nan)

        for column_index, key in enumerate(gen_kw_keys):
            key, keyword = key.split(":")

            use_log_scale = False
            if key.startswith("LOG10_"):
                key = key[6:]
                use_log_scale = True

            ensemble_config_node = ert.ensembleConfig().getNode(key)
            ensemble_data = EnsemblePlotGenKW(ensemble_config_node, fs)
            keyword_index = ensemble_data.getIndexForKeyword(keyword)

            for realization_index, realization_number in enumerate(realizations):
                realization_vector = ensemble_data[realization_number]

                value = realization_vector[keyword_index]

                if use_log_scale:
                    value = math.log10(value)

                gen_kw_array[column_index][realization_index] = value

        gen_kw_data = DataFrame(data=numpy.transpose(gen_kw_array), index=realizations, columns=gen_kw_keys)
        gen_kw_data.index.name = "REALIZATION"

        return gen_kw_data

