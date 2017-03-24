// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_RULE_MATCH_TWO_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_RULE_MATCH_TWO_HH

#include "../config.hh"
#include "../nothing.hh"
#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "rule_match_three.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      // The purpose of rule_match_two is to call all necessary debug hooks of
      // the control class and, if applicable, also call the action class'
      // apply()-method. The latter can be disabled either explicitly (via
      // disable<>) or implicitly by at<> or not_at<>.

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename ... > class Action,
                template< typename ... > class Control,
                bool apply_here = ( ( A == apply_mode::ACTION ) && ( ! is_nothing< Action, Rule >::value ) ) >
      struct rule_match_two;

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_two< Rule, A, M, Action, Control, false >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            Control< Rule >::start( const_cast< const Input & >( in ), st ... );

            if ( rule_match_three< Rule, A, M, Action, Control >::match( in, st ... ) ) {
               Control< Rule >::success( const_cast< const Input & >( in ), st ... );
               return true;
            }
            Control< Rule >::failure( const_cast< const Input & >( in ), st ... );
            return false;
         }
      };

      template< typename Rule, apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control >
      struct rule_match_two< Rule, A, M, Action, Control, true >
      {
         template< typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.template mark< rewind_mode::REQUIRED >();  // TODO: Allow actions to opt-out of receiving input data?

            if ( rule_match_two< Rule, A, rewind_mode::ACTIVE, Action, Control, false >::match( in, st ... ) ) {
               Control< Rule >::template apply< typename Input::action_t, Action >( m.count(), in.count(), in.source(), st ... );
               return m( true );
            }
            return false;
         }
      };

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif