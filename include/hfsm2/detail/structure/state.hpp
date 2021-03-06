namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TStateID,
		  typename TArgs,
		  typename THead>
struct _S {
	static constexpr StateID STATE_ID = TStateID;

	using Args			= TArgs;
	using Head			= THead;

	using PayloadList	= typename Args::PayloadList;

	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT		<Args>;
	using ScopedOrigin	= typename Control::Origin;

	using FullControl	= FullControlT	<Args>;
	using GuardControl	= GuardControlT	<Args>;

	using Empty			= ::hfsm2::detail::Empty<Args>;

	HFSM_INLINE void   deepRegister			(StateRegistry& stateRegistry, const Parent parent);

	HFSM_INLINE bool   deepForwardEntryGuard(GuardControl&)		{ return false; }
	HFSM_INLINE bool   deepEntryGuard		(GuardControl& control);

	HFSM_INLINE void   deepEnterInitial		(Control& control);
	HFSM_INLINE void   deepEnter			(Control& control);

	HFSM_INLINE Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	HFSM_INLINE void   deepReact			(const TEvent& event,
											 FullControl& control);

	HFSM_INLINE bool   deepForwardExitGuard	(GuardControl&)		{ return false; }
	HFSM_INLINE bool   deepExitGuard		(GuardControl& control);

	HFSM_INLINE void   deepExit				(Control& control);

	HFSM_INLINE void   wrapPlanSucceeded	(FullControl& control);
	HFSM_INLINE void   wrapPlanFailed		(FullControl& control);

	HFSM_INLINE void   deepForwardActive	(StateRegistry&, const RequestType)	{}
	HFSM_INLINE void   deepForwardRequest	(StateRegistry&, const RequestType)	{}
	HFSM_INLINE void   deepRequestRemain	(StateRegistry&)					{}
	HFSM_INLINE void   deepRequestRestart	(StateRegistry&)					{}
	HFSM_INLINE void   deepRequestResume	(StateRegistry&)					{}
	HFSM_INLINE void   deepChangeToRequested(StateRegistry&, Control&)			{}

#if defined _DEBUG || defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, Empty>::value;	 }

	static constexpr LongIndex NAME_COUNT = isBare() ? 0 : 1;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	template <typename>
	struct MemberTraits;

	template <typename TReturn, typename TState, typename... Ts>
	struct MemberTraits<TReturn(TState::*)(Ts...)> {
		using State = TState;
	};

	template <typename TMethodType, Method>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(LoggerInterface&) const {}

	template <typename TMethodType, Method TMethodId>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(LoggerInterface& logger) const {
		logger.recordMethod(STATE_ID, TMethodId);
	}
#endif

	// VS	 - error C2079: 'hfsm2::detail::_S<BLAH>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'hfsm2::detail::_S<BLAH>::Head' (aka 'Blah')
	//
	// State 'Blah' hasn't been defined
	Head _head;
	HFSM_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state.inl"
