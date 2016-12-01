#  Copyright (C) 2012  Statoil ASA, Norway.
#
#  The file 'ecl_kw.py' is part of ERT - Ensemble based Reservoir Tool.
#
#  ERT is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.
#
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
#  for more details.
import ctypes
from cwrap import BaseCClass

from ert.enkf import EnkfPrototype, AnalysisConfig, EclConfig, EnkfObs, EnKFState, LocalConfig, ModelConfig, EnsembleConfig, PlotConfig, SiteConfig, ENKF_LIB, EnkfSimulationRunner, EnkfFsManager, ErtWorkflowList, HookManager, HookWorkflow, ESUpdate
from ert.enkf.enums import EnkfInitModeEnum
from ert.enkf.key_manager import KeyManager
from ert.util import SubstitutionList, Log



class EnKFMain(BaseCClass):
    TYPE_NAME = "enkf_main"
    _alloc = EnkfPrototype("void* enkf_main_bootstrap(char*, bool, bool)", bind = False)
    _create_new_config = EnkfPrototype("void enkf_main_create_new_config(char* , char*, char* , int)", bind = False)

    _free = EnkfPrototype("void enkf_main_free(enkf_main)")
    _get_ensemble_size = EnkfPrototype("int enkf_main_get_ensemble_size( enkf_main )")
    _get_ens_config = EnkfPrototype("ens_config_ref enkf_main_get_ensemble_config( enkf_main )")
    _get_model_config = EnkfPrototype("model_config_ref enkf_main_get_model_config( enkf_main )")
    _get_local_config = EnkfPrototype("local_config_ref enkf_main_get_local_config( enkf_main )")
    _get_analysis_config = EnkfPrototype("analysis_config_ref enkf_main_get_analysis_config( enkf_main)")
    _get_site_config = EnkfPrototype("site_config_ref enkf_main_get_site_config( enkf_main)")
    _get_ecl_config = EnkfPrototype("ecl_config_ref enkf_main_get_ecl_config( enkf_main)")
    _get_plot_config = EnkfPrototype("plot_config_ref enkf_main_get_plot_config( enkf_main)")
    _set_eclbase = EnkfPrototype("ui_return_obj enkf_main_set_eclbase( enkf_main, char*)")
    _set_datafile = EnkfPrototype("void enkf_main_set_data_file( enkf_main, char*)")
    _get_schedule_prediction_file = EnkfPrototype("char* enkf_main_get_schedule_prediction_file( enkf_main )")
    _set_schedule_prediction_file = EnkfPrototype("void enkf_main_set_schedule_prediction_file( enkf_main , char*)")
    _get_data_kw = EnkfPrototype("subst_list_ref enkf_main_get_data_kw(enkf_main)")
    _clear_data_kw = EnkfPrototype("void enkf_main_clear_data_kw(enkf_main)")
    _add_data_kw = EnkfPrototype("void enkf_main_add_data_kw(enkf_main, char*, char*)")
    _resize_ensemble = EnkfPrototype("void enkf_main_resize_ensemble(enkf_main, int)")
    _del_node = EnkfPrototype("void enkf_main_del_node(enkf_main, char*)")
    _get_obs = EnkfPrototype("enkf_obs_ref enkf_main_get_obs(enkf_main)")
    _load_obs = EnkfPrototype("void enkf_main_load_obs(enkf_main, char* , bool)")
    _get_pre_clear_runpath = EnkfPrototype("bool enkf_main_get_pre_clear_runpath(enkf_main)")
    _set_pre_clear_runpath = EnkfPrototype("void enkf_main_set_pre_clear_runpath(enkf_main, bool)")
    _iget_keep_runpath = EnkfPrototype("int enkf_main_iget_keep_runpath(enkf_main, int)")
    _iset_keep_runpath = EnkfPrototype("void enkf_main_iset_keep_runpath(enkf_main, int, int_vector)")
    _get_templates = EnkfPrototype("ert_templates_ref enkf_main_get_templates(enkf_main)")
    _get_site_config_file = EnkfPrototype("char* enkf_main_get_site_config_file(enkf_main)")
    _get_history_length = EnkfPrototype("int enkf_main_get_history_length(enkf_main)")
    _get_observations = EnkfPrototype("void enkf_main_get_observations(enkf_main, char*, int, long*, double*, double*)")
    _get_observation_count = EnkfPrototype("int enkf_main_get_observation_count(enkf_main, char*)")
    _iget_state = EnkfPrototype("enkf_state_ref enkf_main_iget_state(enkf_main, int)")
    _get_workflow_list = EnkfPrototype("ert_workflow_list_ref enkf_main_get_workflow_list(enkf_main)")
    _get_hook_manager = EnkfPrototype("hook_manager_ref enkf_main_get_hook_manager(enkf_main)")
    _get_user_config_file = EnkfPrototype("char* enkf_main_get_user_config_file(enkf_main)")
    _get_mount_point = EnkfPrototype("char* enkf_main_get_mount_root( enkf_main )")
    _export_field = EnkfPrototype("bool enkf_main_export_field(enkf_main, char*, char*, bool_vector, enkf_field_file_format_enum, int)")
    _export_field_with_fs = EnkfPrototype("bool enkf_main_export_field_with_fs(enkf_main, char*, char*, bool_vector, enkf_field_file_format_enum, int, enkf_fs_manager)")
    _load_from_forward_model = EnkfPrototype("void enkf_main_load_from_forward_model_from_gui(enkf_main, int, bool_vector, enkf_fs)")
    _create_run_path = EnkfPrototype("void enkf_main_icreate_run_path(enkf_main , run_arg)")
    _submit_simulation = EnkfPrototype("void enkf_main_isubmit_job(enkf_main , run_arg)")
    _alloc_run_context_ENSEMBLE_EXPERIMENT= EnkfPrototype("ert_run_context_obj enkf_main_alloc_ert_run_context_ENSEMBLE_EXPERIMENT( enkf_main , enkf_fs , bool_vector , enkf_init_mode_enum , int)")
    _alloc_field_init_file = EnkfPrototype("cstring_obj enkf_main_alloc_abs_path_to_init_file(enkf_main, enkf_config_node)")
    _get_runpath_list = EnkfPrototype("runpath_list_ref enkf_main_get_runpath_list(enkf_main)")
    _add_node = EnkfPrototype("void enkf_main_add_node(enkf_main, enkf_config_node)")



    def __init__(self, model_config, strict = True, verbose = True):
        c_ptr = self._alloc(model_config, strict, verbose)
        super(EnKFMain, self).__init__(c_ptr)

        # The model_config argument can be None; the only reason to
        # allow that possibility is to be able to test that the
        # site-config loads correctly.
        if model_config is None:
            self.__simulation_runner = None
            self.__fs_manager = None
            self.__es_update = None
        else:
            self.__simulation_runner = EnkfSimulationRunner(self)
            self.__fs_manager = EnkfFsManager(self)
            self.__es_update = ESUpdate(self)


        self.__key_manager = KeyManager(self)

    @staticmethod
    def loadSiteConfig():
        """
        This method will load the site config file; the sole purpose
        of this method is testing.
        """
        EnKFMain( None )


    @classmethod
    def createCReference(cls, c_pointer, parent=None):
        obj = super(EnKFMain, cls).createCReference(c_pointer, parent)
        obj.__simulation_runner = EnkfSimulationRunner(obj)
        obj.__fs_manager = EnkfFsManager(obj)
        return obj


    @staticmethod
    def createNewConfig(config_file, storage_path, dbase_type, num_realizations):
        return EnKFMain._create_new_config(config_file, storage_path, dbase_type, num_realizations)

    def getRealisation(self , iens):
        """ @rtype: EnKFState """
        if 0 <= iens < self.getEnsembleSize():
            return self._iget_state(iens).setParent(self)
        else:
            raise IndexError("iens value:%d invalid Valid range: [0,%d)" % (iens , self.getEnsembleSize()))


    def set_eclbase(self, eclbase):
        self._set_eclbase(eclbase)

    def umount(self):
        if not self.__fs_manager is None:
            self.__fs_manager.umount()

    def free(self):
        self.umount()
        self._free( )

    def getEnsembleSize(self):
        """ @rtype: int """
        return self._get_ensemble_size( )

    def resizeEnsemble(self, value):
        self._resize_ensemble(value)

    def ensembleConfig(self):
        """ @rtype: EnsembleConfig """
        return self._get_ens_config( ).setParent(self)

    def analysisConfig(self):
        """ @rtype: AnalysisConfig """
        return self._get_analysis_config( ).setParent(self)

    def getModelConfig(self):
        """ @rtype: ModelConfig """
        return self._get_model_config( ).setParent(self)

    def logh(self):
        """ @rtype: Log """
        return self._get_logh( ).setParent(self)

    def getLocalConfig(self):
        """ @rtype: LocalConfig """
        config = self._get_local_config( ).setParent(self)
        config.initAttributes( self.ensembleConfig() , self.getObservations() , self.eclConfig().getGrid() )
        return config


    def siteConfig(self):
        """ @rtype: SiteConfig """
        return self._get_site_config( ).setParent(self)

    def eclConfig(self):
        """ @rtype: EclConfig """
        return self._get_ecl_config( ).setParent(self)

    def plotConfig(self):
        """ @rtype: PlotConfig """
        return self._get_plot_config( ).setParent(self)

    def set_datafile(self, datafile):
        self._set_datafile(datafile)

        
    def get_schedule_prediction_file(self):
        schedule_prediction_file = self._get_schedule_prediction_file( )
        return schedule_prediction_file

    def set_schedule_prediction_file(self, file):
        self._set_schedule_prediction_file(file)

    def getDataKW(self):
        """ @rtype: SubstitutionList """
        return self._get_data_kw( )

    def clearDataKW(self):
        self._clear_data_kw( )

    def addDataKW(self, key, value):
        self._add_data_kw(key, value)


    def getMountPoint(self):
        return self._get_mount_point( )


    def del_node(self, key):
        self._del_node(key)

    def getObservations(self):
        """ @rtype: EnkfObs """
        return self._get_obs( ).setParent(self)

    def loadObservations(self , obs_config_file , clear = True):
        self._load_obs(obs_config_file , clear)


    def get_pre_clear_runpath(self):
        pre_clear = self._get_pre_clear_runpath( )
        return pre_clear

    def set_pre_clear_runpath(self, value):
        self._set_pre_clear_runpath(value)

    def iget_keep_runpath(self, iens):
        ikeep = self._iget_keep_runpath(iens)
        return ikeep

    def iset_keep_runpath(self, iens, keep_runpath):
        self._iset_keep_runpath(iens, keep_runpath)

    def get_templates(self):
        return self._get_templates( ).setParent(self)

    def get_site_config_file(self):
        site_conf_file = self._get_site_config_file( )
        return site_conf_file

    def getUserConfigFile(self):
        """ @rtype: str """
        config_file = self._get_user_config_file( )
        return config_file


    def getHistoryLength(self):
        return self._get_history_length( )

    def getMemberRunningState(self, ensemble_member):
        """ @rtype: EnKFState """
        return self._iget_state(ensemble_member).setParent(self)

    def get_observations(self, user_key, obs_count, obs_x, obs_y, obs_std):
        return self._get_observations(user_key, obs_count, obs_x, obs_y, obs_std)

    def get_observation_count(self, user_key):
        return self._get_observation_count(user_key)


    def getESUpdate(self):
        """ @rtype: ESUpdate """
        return self.__es_update

    def getEnkfSimulationRunner(self):
        """ @rtype: EnkfSimulationRunner """
        return self.__simulation_runner

    def getEnkfFsManager(self):
        """ @rtype: EnkfFsManager """
        return self.__fs_manager

    def getKeyManager(self):
        """ :rtype: KeyManager """
        return self.__key_manager

    def getWorkflowList(self):
        """ @rtype: ErtWorkflowList """
        return self._get_workflow_list( ).setParent(self)

    def getHookManager(self):
        """ @rtype: HookManager """
        return self._get_hook_manager( )


    def fieldInitFile(self , config_node):
        return self._alloc_field_init_file( config_node )


    def exportField(self, keyword, path, iactive, file_type, report_step, state, enkfFs):
        """
        @type keyword: str
        @type path: str
        @type iactive: BoolVector
        @type file_type: EnkfFieldFileFormatEnum
        @type report_step: int
        @type enkfFs: EnkfFs

        """
        assert isinstance(keyword, str)
        return self._export_field_with_fs(keyword, path, iactive, file_type, report_step, state, enkfFs)

    def loadFromForwardModel(self, realization, iteration, fs):
        self._load_from_forward_model(iteration, realization, fs)

    def createRunPath(self , run_arg):
        self._create_run_path( run_arg)

    def submitSimulation(self , run_arg):
        self._submit_simulation( run_arg)


    def getRunContextENSEMPLE_EXPERIMENT(self , fs , iactive , init_mode = EnkfInitModeEnum.INIT_CONDITIONAL , iteration = 0):
        return self._alloc_run_context_ENSEMBLE_EXPERIMENT( fs , iactive , init_mode , iteration )


    def getRunpathList(self):
        return self._get_runpath_list( )

    def addNode(self, enkf_config_node):
        self._add_node(enkf_config_node)


