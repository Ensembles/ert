#ifndef OPM_ERT_SMSPEC_HPP
#define OPM_ERT_SMSPEC_HPP

#include <memory>
#include <string>

#include <ert/ecl/smspec_node.h>
#include <ert/util/ert_unique_ptr.hpp>

namespace ERT {

    class smspec_node {
        public:
            smspec_node(
                    ecl_smspec_var_type,
                    const std::string& wgname,
                    const std::string& keyword
            );

            smspec_node(
                    ecl_smspec_var_type,
                    const std::string& wgname,
                    const std::string& keyword,
                    const std::string& join
            );

            const char* wgname() const;

        private:
            smspec_node(
                ecl_smspec_var_type,
                const char*, const char*, const char*, const char*,
                const int[3], int, int = 0, float = 0 );

            ert_unique_ptr< smspec_node_type, smspec_node_free > node;
    };

}

#endif //OPM_ERT_SMSPEC_HPP
