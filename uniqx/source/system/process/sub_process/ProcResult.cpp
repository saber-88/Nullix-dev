#include <sys/wait.h>

#include "sub_process/ProcResult.hpp"

namespace [[
    
    ]] uniqx
{
    
    using namespace system::process;
    
    auto
        ProcResult::exited
        ( void /* v_ */ ) const
    -> bool
    {
        return ( WIFEXITED ( this->_status ) );
    }
    
    auto
        ProcResult::signaled
        ( void /* v_ */ ) const
    -> bool
    {
        return ( WIFSIGNALED ( this->_status ) );
    }
    
    auto
        ProcResult::stopped
        ( void /* v_ */ ) const
    -> bool
    {
        return ( WIFSTOPPED ( this->_status ) );
    }
    
    auto
        ProcResult::continued
        ( void /* v_ */ ) const
    -> bool
    {
        #ifdef WIFCONTINUED
            return ( WIFCONTINUED ( this->_status ) );
        #else
            return ( false );
        #endif
    }
    
    auto
        ProcResult::core_dumped
        ( void /* v_ */ ) const
    -> bool
    {
        #if defined WCOREDUMP
            return ( this->signaled ( ) && WCOREDUMP ( this->_status ) );
        #else
            return ( false );
        #endif
    }
    
    auto
        ProcResult::stop_signal
        ( void /* v_ */ ) const
    -> int
    {
        return ( this->stopped ( ) ? WSTOPSIG ( this->_status ) : -1 );
    }
    
    auto
        ProcResult::term_signal
        ( void /* v_ */ ) const
    -> int
    {
        return ( this->signaled ( ) ? WTERMSIG ( this->_status ) : -1 );
    }
    
    auto
        ProcResult::exit_code
        ( void /* v_ */ ) const
    -> int
    {
        return ( this->exited ( ) ? WEXITSTATUS ( this->_status ) : -1 );
    }
    
    auto
        ProcResult::stdOut
        ( void /* v_ */ ) const
    -> std::string
    {
        return ( std::move ( this->_stdout ) );
    }
    
    auto
        ProcResult::stdErr
        ( void /* v_ */ ) const
    -> std::string
    {
        return ( std::move ( this->_stderr ) );
    }
    
    auto
        ProcResult::success
        ( void /* v_ */ ) const
    -> bool
    {
        return ( ( this->exited ( ) && this->exit_code ( ) ) == +0 );
    }
    
    template<std::predicate<const ProcResult&> Rule>
    auto
        ProcResult::success
        ( Rule&& p_rule_ ) const
    -> bool
    {
        return ( p_rule_ ( *this ) );
    }
    

        ProcResult::operator int
        ( void /* v_ */ ) const
    {
        return ( static_cast<int> ( this->_status ) );
    }
    

        ProcResult::operator bool
        ( void /* v_ */ ) const
    {
        return ( this->success ( ) );
    }
    

        ProcResult::operator long
        ( void /* v_ */ ) const
    {
        return ( this->_status );
    }
    
} /* namespace uniqx */
