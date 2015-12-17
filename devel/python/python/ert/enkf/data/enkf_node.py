#  Copyright (C) 2012  Statoil ASA, Norway. 
#   
#  The file 'enkf_node.py' is part of ERT - Ensemble based Reservoir Tool. 
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
import sys

from ert.cwrap import BaseCClass, CWrapper
from ert.enkf import ENKF_LIB, EnkfFs, NodeId
from ert.enkf.data import GenKw, GenData, CustomKW, Field
from ert.enkf.enums import ErtImplType,EnkfStateType

class EnkfNode(BaseCClass):
    def __init__(self, config_node, private=False):
        if private:
            c_pointer = EnkfNode.cNamespace().alloc_private(config_node)
        else:
            c_pointer = EnkfNode.cNamespace().alloc(config_node)

        super(EnkfNode, self).__init__(c_pointer, config_node, True)

    @classmethod
    def exportMany(cls , config_node , file_format , fs , iens_list  , state = EnkfStateType.ANALYZED , report_step = 0 , file_type = None , arg = None):
        node = EnkfNode( config_node )
        for iens in iens_list:
            filename = file_format % iens
            node_id = NodeId( report_step , iens , state )
            if node.tryLoad(fs , node_id):
                if node.export( filename , file_type = file_type , arg = arg):
                    print("%s[%03d] -> %s" % (config_node.getKey() , iens , filename))
            else:
                sys.stderr.write("** ERROR: Could not load realisation:%d - export failed" % iens)


    def export(self , filename , file_type = None , arg = None):
        impl_type = self.getImplType()
        if impl_type == ErtImplType.FIELD:
            field_node = self.asField( )
            return field_node.export( filename , file_type = file_type , init_file = arg)
        else:
            raise NotImplementedError("The export method is only implemented for field")



    def valuePointer(self):
        return EnkfNode.cNamespace().value_ptr(self)

    def getImplType(self):
        """ @rtype: ert.enkf.enums.ert_impl_type_enum.ErtImplType """
        return EnkfNode.cNamespace().get_impl_type(self)

    
    def asGenData(self):
        """ @rtype: GenData """
        impl_type = self.getImplType( )
        assert impl_type == ErtImplType.GEN_DATA

        return GenData.createCReference(self.valuePointer(), self)

    def asGenKw(self):
        """ @rtype: GenKw """
        impl_type = self.getImplType( )
        assert impl_type == ErtImplType.GEN_KW

        return GenKw.createCReference(self.valuePointer(), self)

    def asCustomKW(self):
        """ @rtype: CustomKW """
        impl_type = self.getImplType( )
        assert impl_type == ErtImplType.CUSTOM_KW

        return CustomKW.createCReference(self.valuePointer(), self)

    def asField(self):
        """ @rtype: CustomKW """
        impl_type = self.getImplType( )
        assert impl_type == ErtImplType.FIELD

        return Field.createCReference(self.valuePointer(), self)

    def tryLoad(self, fs, node_id):
        """
        @type fs: EnkfFS
        @type node_id: NodeId
        @rtype: bool
        """
        assert isinstance(fs, EnkfFs)
        assert isinstance(node_id, NodeId)

        return EnkfNode.cNamespace().try_load(self, fs, node_id)

    def name(self):
        """ @rtype: str """
        return EnkfNode.cNamespace().get_name(self)

    def load(self, fs, node_id):
        if not self.tryLoad(fs, node_id):
            raise Exception("Could not load node: %s iens: %d report: %d" % (self.name(), node_id.iens, node_id.report_step))

    def save(self, fs, node_id):
        assert isinstance(fs, EnkfFs)
        assert isinstance(node_id, NodeId)
        
        EnkfNode.cNamespace().store(self, fs, True, node_id)

    def free(self):
        EnkfNode.cNamespace().free(self)


cwrapper = CWrapper(ENKF_LIB)
cwrapper.registerObjectType("enkf_node", EnkfNode)

EnkfNode.cNamespace().free = cwrapper.prototype("void enkf_node_free(enkf_node)")
EnkfNode.cNamespace().alloc = cwrapper.prototype("c_void_p enkf_node_alloc(enkf_config_node)")
EnkfNode.cNamespace().alloc_private = cwrapper.prototype("c_void_p enkf_node_alloc_private_container(enkf_config_node)")
EnkfNode.cNamespace().get_name = cwrapper.prototype("char* enkf_node_get_key(enkf_node)")

EnkfNode.cNamespace().value_ptr = cwrapper.prototype("c_void_p enkf_node_value_ptr(enkf_node)")

EnkfNode.cNamespace().try_load = cwrapper.prototype("bool enkf_node_try_load(enkf_node, enkf_fs, node_id)")
EnkfNode.cNamespace().get_impl_type = cwrapper.prototype("ert_impl_type_enum enkf_node_get_impl_type(enkf_node)")
EnkfNode.cNamespace().store = cwrapper.prototype("void enkf_node_store(enkf_node, enkf_fs, bool, node_id)")
