/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_req_header_state.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 11:16:58 by alephoen          #+#    #+#             */
/*   Updated: 2026/05/29 11:16:58 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <b_request/HTTParser.hpp>
#include <d_client/Client.hpp>

#include <cstdio>

HEADER_STATE::STATE
HTTParser::check_req_header_state(Client &c, char *buffer, SIZET byts_r)
{
    STATE_MACHINE::STATE buf_state;
	
	SIZET	byts_cnt;

    buf_state = check_req_buf_state(buffer, byts_r, this->state_, &byts_cnt);

	if (c.r_buf_size_ >= MAX_READ_BUFFER_SIZE)
		return HEADER_STATE::HEAD_FATAL_ERR;

    switch (buf_state)
    {
        case STATE_MACHINE::NO_CR:
        case STATE_MACHINE::IS_CR:
        case STATE_MACHINE::IS_CRLF:
        case STATE_MACHINE::IS_CRLF_CR:
        case STATE_MACHINE::IS_CRLF_CRLF:
            c.r_buf_.append(buffer, byts_cnt);
            c.r_buf_size_ = c.r_buf_.size();
			if (buf_state == STATE_MACHINE::IS_CRLF_CRLF)
				this->state_ = STATE_MACHINE::NO_CR;
            break;
        case STATE_MACHINE::STATE_FATAL_ERR:
            return HEADER_STATE::HEAD_FATAL_ERR;
        default:
			this->state_ = STATE_MACHINE::STATE_FATAL_ERR;
            return HEADER_STATE::HEAD_FATAL_ERR;
    }

    switch (buf_state)
    {
        case STATE_MACHINE::NO_CR:
            return HEADER_STATE::NEED_MORE_DATA;
        case STATE_MACHINE::IS_CR:
            return HEADER_STATE::NEED_MORE_DATA;
        case STATE_MACHINE::IS_CRLF:
            return HEADER_STATE::HEADER_COMPLET;
        case STATE_MACHINE::IS_CRLF_CR:
            return HEADER_STATE::NEED_MORE_DATA;
        case STATE_MACHINE::IS_CRLF_CRLF:
            return HEADER_STATE::END_OF_HEADERS;
        case STATE_MACHINE::STATE_FATAL_ERR:
            return HEADER_STATE::HEAD_FATAL_ERR;
        default:
            return HEADER_STATE::HEAD_FATAL_ERR;
    }

    return HEADER_STATE::HEAD_FATAL_ERR;
}

STATE_MACHINE::STATE	HTTParser::
check_req_buf_state(char *buffer, SIZET byts_r, 
					STATE_MACHINE::STATE &state, SIZET *byts_cnt)
{
	if (!byts_r)
		{ *byts_cnt = 0; return state; }

	r_buf_pos_	= (SIZET)-1;
	*byts_cnt	= 0;

	SIZET *i = &r_buf_pos_;
    while (++*i < byts_r)
    {
		++(*byts_cnt);

        switch (state)
        {
            case STATE_MACHINE::NO_CR:
                if (buffer[*i] == '\r')
                    state = STATE_MACHINE::IS_CR;
                break;

            case STATE_MACHINE::IS_CR:
                if (buffer[*i] != '\n')
				{
					state = STATE_MACHINE::STATE_FATAL_ERR;
                    return STATE_MACHINE::STATE_FATAL_ERR;
				}
                state = STATE_MACHINE::IS_CRLF;
                break;

            case STATE_MACHINE::IS_CRLF:
                if (buffer[*i] != '\r')
					{ --(*i); *byts_cnt -= 1; return STATE_MACHINE::IS_CRLF; }
                state = STATE_MACHINE::IS_CRLF_CR;
                break;

            case STATE_MACHINE::IS_CRLF_CR:
                if (buffer[*i] != '\n')
				{
					state = STATE_MACHINE::STATE_FATAL_ERR;
                    return STATE_MACHINE::STATE_FATAL_ERR;
				}
                state = STATE_MACHINE::IS_CRLF_CRLF;
				return state;
                break;

            default:
				state = STATE_MACHINE::STATE_FATAL_ERR;
                return STATE_MACHINE::STATE_FATAL_ERR;
        }
    }
	--(*i);
    return state;
}

