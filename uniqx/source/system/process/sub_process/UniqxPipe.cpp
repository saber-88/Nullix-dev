#include <array>
#include <print>
#include <cstdlib>

#include "SubProcess.hpp"
#include "sub_process/UniqxPd.hpp"
#include "sub_process/UniqxPipe.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    using namespace system::process;
    
    auto SubProcess::
        UniqxPipe::create
        ( void /* v_ */ )
    -> UniqxPipe
    {
        
        constexpr int K_i_pipeDes_RX { +0 };
        constexpr int K_i_pipeDes_TX { +1 };
        constexpr int K_i_pipeDesLim { +2 };
        
        std::array<int , K_i_pipeDesLim> _i_arr_pipeDes { };
        
        if
            ( ::pipe ( _i_arr_pipeDes.data ( ) ) == -1 )
        {
            std::println ( stderr , "[Err]: Unable to create pipe" );
            std::exit ( errno );
        }
        
        return
            {
                .pipeDes_RX
                {
                    UniqxPd
                    {
                        _i_arr_pipeDes.at ( K_i_pipeDes_RX )
                    }
                } ,
                .pipeDes_TX
                {
                    UniqxPd
                    {
                        _i_arr_pipeDes.at ( K_i_pipeDes_TX )
                    }
                }
            }
        ;
        
    }
    
} /* namespace uniqx */
