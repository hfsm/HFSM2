﻿#pragma once

namespace hfsm2 {

//------------------------------------------------------------------------------

template <LongIndex NMaxPlanTasks = INVALID_LONG_INDEX,
		  LongIndex NMaxSubstitutions = 4>
struct Config {
	static constexpr LongIndex MAX_PLAN_TASKS	 = NMaxPlanTasks;
	static constexpr LongIndex MAX_SUBSTITUTIONS = NMaxSubstitutions;
};

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct _SF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct _CSF;

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _CSF<TInitial>;

template <typename, typename...>
struct _CF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct _OSF;

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...>;

template <typename TInitial>
struct _OSF<TInitial>;

template <typename, typename...>
struct _OF;

//------------------------------------------------------------------------------

template <typename...>
struct WrapForward;

template <typename TH>
struct WrapForward<TH> {
	using Type = _SF<TH>;
};

template <typename TH, typename... TS>
struct WrapForward<_CF<TH, TS...>> {
	using Type = _CF<TH, TS...>;
};

template <typename TH, typename... TS>
struct WrapForward<_OF<TH, TS...>> {
	using Type = _OF<TH, TS...>;
};

//------------------------------------------------------------------------------

template <typename THead>
struct _SF final {
	using Head				= THead;
	using StateList			= _TL<Head>;
	using RegionList		= _TL<>;

	static constexpr ShortIndex WIDTH		  = 1;
	static constexpr LongIndex  REVERSE_DEPTH = 1;
	static constexpr LongIndex  DEEP_WIDTH	  = 0;
	static constexpr ShortIndex COMPO_COUNT	  = 0;
	static constexpr ShortIndex ORTHO_COUNT	  = 0;
	static constexpr ShortIndex ORTHO_UNITS	  = 0;
	static constexpr LongIndex  PRONG_COUNT	  = 0;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _CSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _CSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList,  typename Remaining::StateList >::TypeList;
	using RegionList		= typename MergeT<typename Initial::RegionList, typename Remaining::RegionList>::TypeList;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex  DEEP_WIDTH	  = Max<Initial::DEEP_WIDTH,	Remaining::DEEP_WIDTH	>::VALUE;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT + Remaining::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT + Remaining::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS + Remaining::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT + Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _CSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr LongIndex  DEEP_WIDTH	  = Max<1, Initial::DEEP_WIDTH>::VALUE;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT;
};

template <typename THead, typename... TSubStates>
struct _CF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _CSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList >::TypeList;
	using RegionList		= typename MergeT<typename State::StateList, typename SubStates::RegionList>::TypeList;

	static constexpr ShortIndex WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex  REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex  DEEP_WIDTH	  = SubStates::DEEP_WIDTH;
	static constexpr ShortIndex COMPO_COUNT	  = SubStates::COMPO_COUNT + 1;
	static constexpr ShortIndex ORTHO_COUNT	  = SubStates::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = SubStates::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = WIDTH + SubStates::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct _OSF<TInitial, TRemaining...> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using Remaining			= _OSF<TRemaining...>;
	using StateList			= typename MergeT<typename Initial::StateList,  typename Remaining::StateList >::TypeList;
	using RegionList		= typename MergeT<typename Initial::RegionList, typename Remaining::RegionList>::TypeList;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH, Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr LongIndex  DEEP_WIDTH	  = Initial::DEEP_WIDTH  + Remaining::DEEP_WIDTH;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT + Remaining::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT + Remaining::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS + Remaining::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT + Remaining::PRONG_COUNT;
};

template <typename TInitial>
struct _OSF<TInitial> {
	using Initial			= typename WrapForward<TInitial>::Type;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr LongIndex  DEEP_WIDTH	  = Initial::DEEP_WIDTH;
	static constexpr ShortIndex COMPO_COUNT	  = Initial::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = Initial::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
	static constexpr LongIndex  PRONG_COUNT	  = Initial::PRONG_COUNT;
};

template <typename THead, typename... TSubStates>
struct _OF final {
	using Head				= THead;
	using State				= _SF<Head>;
	using SubStates			= _OSF<TSubStates...>;
	using StateList			= typename MergeT<typename State::StateList, typename SubStates::StateList >::TypeList;
	using RegionList		= typename MergeT<typename State::StateList, typename SubStates::RegionList>::TypeList;

	static constexpr ShortIndex WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex  REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr LongIndex  DEEP_WIDTH	  = SubStates::DEEP_WIDTH;
	static constexpr ShortIndex COMPO_COUNT	  = SubStates::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT	  = SubStates::ORTHO_COUNT + 1;
	static constexpr ShortIndex ORTHO_UNITS	  = SubStates::ORTHO_UNITS + (WIDTH + 7) / 8;
	static constexpr LongIndex  PRONG_COUNT	  = SubStates::PRONG_COUNT;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct ArgsT final {
	using Context			= TContext;
	using Config			= TConfig;
	using StateList			= TStateList;
	using RegionList		= TRegionList;
	using PayloadList		= TPayloadList;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT	  = NCompoCount;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  TASK_CAPACITY = NTaskCapacity;
};

//------------------------------------------------------------------------------

template <StateID, typename, typename>
struct _S;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _C;

template <StateID, ShortIndex, ShortIndex, typename, typename, typename...>
struct _O;

template <typename, typename, typename, typename>
class _R;

//------------------------------------------------------------------------------

template <StateID, ShortIndex, ShortIndex, typename...>
struct WrapMaterial;

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH>
struct WrapMaterial<NS, NC, NO, TA, TH> {
	using Type = _S<NS,			TA, TH>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _CF<void, TS...>> {
	using Type = _C<NS, NC, NO, TA, Empty<TA>, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _CF<TH,	 TS...>> {
	using Type = _C<NS, NC, NO, TA, TH,		  TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA,			  typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _OF<void, TS...>> {
	using Type = _O<NS, NC, NO, TA, Empty<TA>, TS...>;
};

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
struct WrapMaterial<NS, NC, NO, TA, _OF<TH,	 TS...>> {
	using Type = _O<NS, NC, NO, TA, TH,		  TS...>;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TPayloadList,
		  typename TApex>
struct _RF final {
	using Context			= TContext;
	using Config			= TConfig;
	using PayloadList		= TPayloadList;
	using Apex				= TApex;

	static constexpr LongIndex MAX_PLAN_TASKS	 = Config::MAX_PLAN_TASKS;
	static constexpr LongIndex MAX_SUBSTITUTIONS = Config::MAX_SUBSTITUTIONS;

	static constexpr LongIndex TASK_CAPACITY	 = Config::MAX_PLAN_TASKS != INVALID_LONG_INDEX ?
													   Config::MAX_PLAN_TASKS : Apex::PRONG_COUNT * 2;

	using Instance			= _R<Context, Config, PayloadList, Apex>;

	static constexpr ShortIndex COMPO_COUNT		 = Apex::COMPO_COUNT;
	static constexpr ShortIndex ORTHO_COUNT		 = Apex::ORTHO_COUNT;
	static constexpr ShortIndex ORTHO_UNITS		 = Apex::ORTHO_UNITS;

	using StateList			= typename Apex::StateList;
	using RegionList		= typename Apex::RegionList;

	using Args				= ArgsT<Context,
									Config,
									StateList,
									RegionList,
									COMPO_COUNT,
									ORTHO_COUNT,
									ORTHO_UNITS,
									PayloadList,
									TASK_CAPACITY>;

	using Control			= ControlT	   <Args>;
	using FullControl		= FullControlT <Args>;
	using GuardControl		= GuardControlT<Args>;

	using Bare				= BareT<Args>;
	using State				= Empty<Args>;

	template <typename... TInjections>
	using BaseT = _B<TInjections...>;

	template <typename T>
	static constexpr LongIndex stateId() {
		return StateList::template index<T>();
	}

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG_INDEX;
	}

	template <typename T>
	static constexpr LongIndex regionId() {
		return RegionList::template index<T>();
	}
};

////////////////////////////////////////////////////////////////////////////////

}
}
