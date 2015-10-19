from types import NoneType
from ert.cwrap import BaseCClass, CWrapper
from ert.enkf import ENKF_LIB
from ert.util import Matrix


class ObsData(BaseCClass):

    def __init__(self, global_std_scaling=1.0):
        c_pointer = ObsData.cNamespace().alloc(global_std_scaling)
        super(ObsData, self).__init__(c_pointer)

    def __len__(self):
        """ @rtype: int """
        return ObsData.cNamespace().active_size(self)


    def addBlock(self , obs_key , obs_size):
        error_covar = None 
        error_covar_owner = False
        return ObsData.cNamespace().add_block(self , obs_key , obs_size , error_covar , error_covar_owner)


    def createDObs(self):
        """ @rtype: Matrix """
        return ObsData.cNamespace().allocdObs(self)

    def createR(self):
        """ @rtype: Matrix """
        return ObsData.cNamespace().allocR(self)

    def createD(self , E , S):
        """ @rtype: Matrix """
        return ObsData.cNamespace().allocD(self , E , S)

    def createE( self , rng , active_ens_size):
        """ @rtype: Matrix """
        return ObsData.cNamespace().allocE(self , rng , active_ens_size)

    def scaleMatrix(self, m):
        ObsData.cNamespace().scale_matrix(self , m )


    def scaleRMatrix(self, R):
        ObsData.cNamespace().scale_Rmatrix(self , R )


    def scale(self, S, E=None, D=None, R=None, D_obs=None):
        assert isinstance(S, Matrix)
        assert isinstance(E, (Matrix, NoneType))
        assert isinstance(D, (Matrix, NoneType))
        assert isinstance(R, (Matrix, NoneType))
        assert isinstance(D_obs, (Matrix, NoneType))
        ObsData.cNamespace().scale(self, S, E, D, R, D_obs)
        
    def getStd(self, block_index, obs_index):
        assert isinstance(block_index, int)
        assert isinstance(obs_index, int)
        block = ObsData.cNamespace().get_block(self, block_index)
        return ObsData.cNamespace().get_block_std(block, obs_index)


    def free(self):
        ObsData.cNamespace().free(self)



cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerObjectType("obs_data", ObsData)

ObsData.cNamespace().alloc       = cwrapper.prototype("c_void_p obs_data_alloc(double)")
ObsData.cNamespace().free        = cwrapper.prototype("void obs_data_free(obs_data)")
ObsData.cNamespace().active_size = cwrapper.prototype("int obs_data_get_active_size(obs_data)")
ObsData.cNamespace().add_block   = cwrapper.prototype("obs_block_ref obs_data_add_block(obs_data , char* , int , matrix , bool)")

ObsData.cNamespace().allocdObs   = cwrapper.prototype("matrix_obj obs_data_allocdObs(obs_data)")
ObsData.cNamespace().allocR      = cwrapper.prototype("matrix_obj obs_data_allocR(obs_data)")
ObsData.cNamespace().allocD      = cwrapper.prototype("matrix_obj obs_data_allocD(obs_data , matrix , matrix)")
ObsData.cNamespace().allocE      = cwrapper.prototype("matrix_obj obs_data_allocE(obs_data , rng , int)")
ObsData.cNamespace().scale       = cwrapper.prototype("void obs_data_scale(obs_data, matrix, matrix, matrix, matrix, matrix)")
ObsData.cNamespace().scale_matrix = cwrapper.prototype("void obs_data_scale_matrix(obs_data, matrix)")
ObsData.cNamespace().scale_Rmatrix = cwrapper.prototype("void obs_data_scale_Rmatrix(obs_data, matrix)")

ObsData.cNamespace().get_block_std = cwrapper.prototype("double obs_block_iget_std(obs_block , int)")
ObsData.cNamespace().get_block     = cwrapper.prototype("obs_block_ref obs_data_iget_block(obs_data, int)")


