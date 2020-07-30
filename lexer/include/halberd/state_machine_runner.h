#pragma once

#include "state_index.h"
#include "state_machine_array.h"

#include <algorithm> // std::binary_search


namespace halberd
{
namespace lexer
{
    template<typename TSym>
    constexpr bool try_transition(const state_transition_view<TSym>& stv, TSym sym)
    {
        //TODO: may need to implement binary_search as std version not guaranteed constepxr until C++20
        return std::binary_search(
            stv.begin(),
            stv.end(),
            sym);
    }

    template<typename TSym, typename TTag>
    constexpr bool try_transition(const state_machine_view<TSym, TTag>& smv, TSym sym, state_index_t& idx_state)
    {
        if (state_index_invalid == idx_state)
        {
            throw std::exception(); //TODO: don't use exceptions here so the function can be noexcept?
        }

        const auto& sv = smv[idx_state];

        auto it_trans = sv.begin();
        const auto it_end = sv.end();

        while ((it_trans != it_end) && !try_transition(*it_trans, sym))
        {
            ++it_trans;
        }

        if (it_trans != it_end)
        {
            idx_state = it_trans->idx_to;
        }
        else
        {
            idx_state = state_index_invalid;
        }

        return idx_state != state_index_invalid;
    }

    template<typename TSym, typename TTag>
    class state_machine_runner
    {
    public:
        state_machine_runner(state_machine_view<TSym, TTag> smv) : smv(std::move(smv)), _idx_state(smv.idx_start)
        {
        }

        bool try_transition(TSym sym)
        {
            //TODO: what if the current state index is invalid? throw an exception? (try_transition already does that)?
            return halberd::lexer::try_transition(smv, sym, _idx_state);
        }

        bool is_state_valid() const
        {
            return state_index_invalid != _idx_state;
        }

        bool is_state_start() const
        {
            return is_state_valid() && smv.idx_start == _idx_state;
        }

        bool is_state_accepting() const
        {
            return is_state_valid() && smv[_idx_state].is_accept_state;
        }

        TTag get_state_tag() const
        {
            if (is_state_valid())
            {
                return smv[_idx_state].tag;
            }
            else
            {
                throw std::exception();
            }
        }

        void reset()
        {
            _idx_state = smv.idx_start;
        }

        state_machine_view<TSym, TTag> smv;

    private:
        state_index_t _idx_state;
    };
}
}
