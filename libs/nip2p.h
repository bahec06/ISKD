#ifndef ___nip2p_h___
#define ___nip2p_h___

#ifndef NIP2P_FUNC
#   if (defined(__cplusplus) || defined(__cplusplus__))
#      define NIP2P_EXTERNC extern "C"
#   else
#      define NIP2P_EXTERNC
#   endif

#   if defined(_MSC_VER) || defined(_CVI_)
#      define NIP2P_FUNC NIP2P_EXTERNC int32_t __cdecl
#   elif defined(__GNUC__)
#      define NIP2P_FUNC NIP2P_EXTERNC int32_t
#   else
#      error Unsupported compiler!
#   endif

#   if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) \
     || defined(__GNUC__) || defined(__vxworks)
#      include <stdint.h>
#   elif defined(_CVI_)
       typedef unsigned char uint8_t;
       typedef signed   int  int32_t;
       typedef unsigned int  uint32_t;
#   elif defined(_MSC_VER)
       typedef unsigned __int8  uint8_t;
       typedef __int32          int32_t;
       typedef unsigned __int32 uint32_t;
#   else
#      error Unsupported compiler!
#   endif
#endif

//#include "PublicIncludes/nistreamCommon/prefix.h"

//============================================================================
//  NI-P2P Types and Enums
//============================================================================

//============================================================================
//  tEndpointHandle - Identifies an endpoint resource.  The native driver for
//      an endpoint is responsible for providing an API to return one of these.
//============================================================================
typedef uint32_t nip2p_tEndpointHandle;


//============================================================================
//  tStreamHandle - Identifies a single stream session instance.
//============================================================================
typedef uint32_t nip2p_tStreamHandle;


//============================================================================
//  tStreamState - Defines the various states of a stream.  These states
//      are all fully defined in the P2P State Model.  Brief definition of
//      each state follows:
//
//          kStreamStateUnlinked - The stream has awareness of which endpoints
//              are participating in the stream, but they are not yet linked.
//          kStreamStateDisabled - The stream and its two endpoints are fully
//              linked, but the stream is still idle.  When in this state,
//              no data is permitted to flow across the stream connection.
//          kStreamStateEnabled - The stream connection is enabled and fully
//              capable of transferring data.  Note that this doesn't necessarily
//              mean that the stream is actively transferring data -- only that
//              it is capable of doing so.
//          kStreamStateFlushing - The stream is in the process of disabling.
//              In this state, the stream is waiting for all pending data
//              on the writer endpoint to be flushed across to the reader
//              endpoint.
//============================================================================
typedef enum {
    nip2p_kStreamStateUnlinked = 0L,
    nip2p_kStreamStateDisabled = 1L,
    nip2p_kStreamStateEnabled  = 2L,
    nip2p_kStreamStateFlushing = 3L,
} nip2p_tStreamState;


//============================================================================
//  tStreamAttribute - Defines the various attributes that can be accessed
//      on a stream session.  These attributes fall into three different
//      categories:  common stream attributes, writer endpoint-specific
//      attributes and reader endpoint-specific attributes.
//============================================================================
typedef enum {
    //------------------------------------------------------------------------
    //  Common Stream Attributes
    //------------------------------------------------------------------------
    nip2p_kStreamAttributeStreamState                 = 0x00000000L,
    //------------------------------------------------------------------------
    //  Writer Endpoint Attributes
    //------------------------------------------------------------------------
    nip2p_kStreamAttributeWriterNumElementsForWriting = 0x10000000L,
    nip2p_kStreamAttributeWriterSizeInElements        = 0x10000001L,
    nip2p_kStreamAttributeWriterOverflow              = 0x10000002L,
    //------------------------------------------------------------------------
    //  Reader Endpoint Attributes
    //------------------------------------------------------------------------
    nip2p_kStreamAttributeReaderNumElementsForReading = 0x20000000L,
    nip2p_kStreamAttributeReaderSizeInElements        = 0x20000001L,
    nip2p_kStreamAttributeReaderUnderflow             = 0x20000002L,
} nip2p_tStreamAttribute;

//============================================================================
//  tStreamEvent - Defines the various type of stream events that can occur.
//      You can wait for these events by calling nip2pWaitForStreamEvent.
//============================================================================
typedef enum {
    nip2p_kStreamEventFIFOOverflow   = 0L,
    nip2p_kStreamEventFIFOUnderflow  = 1L,
    nip2p_kStreamEventStreamEnabled  = 2L,
    nip2p_kStreamEventStreamDisabled = 3L,
} nip2p_tStreamEvent;


//============================================================================
//  NI P2P Functions
//============================================================================

//////////////////////////////////////////////////////////////////////////////
//
//  nip2pCreateAndLinkStream
//
//  Description:
//      Creates a new stream session that uses the provided writer and reader
//      endpoint resources and links them together (which entails actual
//      communication with hardware).  After linkage, each physical endpoint
//      will have enough information programmed into it to be aware of each
//      other's existence.  Upon return, the public handle to the stream
//      session is returned via the streamHandle parameter.  This function
//      will also optionally enable the stream (specified by the enableStream
//      flag).  Upon return, this function leaves the stream session in either
//      the kStreamStateDisabled or kStreamStateEnabled state, depending on the
//      value of the enableStream flag.
//
//  Parameters:
//      writerEndpointHandle - Handle to the writer endpoint resource
//      readerEndpointHandle - Handle to the reader endpoint resource
//      enableStream         - If non-zero, we'll immediately enable the stream
//                             after we link the endpoints.  If zero, we will
//                             only link the endpoints.
//      streamHandle         - Upon return, contains the public handle to the
//                             newly created stream session if it was created
//                             successfully.  Contains an empty handle if the
//                             stream session wasn't created successfully.
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pCreateAndLinkStream(nip2p_tEndpointHandle writerEndpointHandle,
                                    nip2p_tEndpointHandle readerEndpointHandle,
                                    uint8_t enableStream,
                                    nip2p_tStreamHandle* streamHandle);

//////////////////////////////////////////////////////////////////////////////
//
//  nip2pDestroyStream
//
//  Description:
//      Destroys the stream session which is identified by the provided
//      streamHandle.  This function will automatically tear down any necessary
//      stream resources.  For example, if the stream is currently enabled,
//      this method will automatically disable and unlink the endpoints before
//      destroying the session.
//
//  Parameters:
//      streamHandle - The stream handle to destroy
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pDestroyStream(nip2p_tStreamHandle streamHandle);

//////////////////////////////////////////////////////////////////////////////
//
//  nip2pLinkStream
//
//  Description:
//      Links the stream session identified by the given streamHandle.  This
//      function will auto-transition through any necessary prerequisite states.
//      If the stream is already linked, this function does nothing.
//
//  Parameters:
//      streamHandle - The stream handle to link
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pLinkStream(nip2p_tStreamHandle streamHandle);


//////////////////////////////////////////////////////////////////////////////
//
//  nip2pEnableStream
//
//  Description:
//      Enables the stream session identified by the given streamHandle.  This
//      function will auto-transition through any necessary prerequisite states.
//      If this stream is already enabled, this function does nothing.
//
//  Parameters:
//      streamHandle - The stream handle to enable
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pEnableStream(nip2p_tStreamHandle streamHandle);


//////////////////////////////////////////////////////////////////////////////
//
//  nip2pDisableStream
//
//  Description:
//      Immediately disables the stream session identified by the given streamHandle.
//      Any residual data currently in the writer endpoint's FIFO will be
//      lost.  If you are interested in this residual data making its way to
//      the reader endpoint, you should use the nip2pFlushAndDisableStream function.
//      This function will auto-transition through any necessary prerequisite
//      states.  If the stream is already disabled, this function does nothing.
//
//  Parameters:
//      streamHandle - The stream handle to disable
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pDisableStream(nip2p_tStreamHandle streamHandle);


//////////////////////////////////////////////////////////////////////////////
//
//  nip2pFlushAndDisableStream
//
//  Description:
//      Disables the stream session identified by the given streamHandle and
//      requests that any residual data already in the writer endpoint's data
//      pipe be flushed to the reader endpoint before actually halting data
//      transfer.  This function will auto-transition through any necessary
//      prerequisite states.  If the stream is already disabled, this function
//      does nothing.
//
//      If the flush operation times out, the flushTimedOut parameter will return
//      with a non-zero value.  We do not return a fatal status on flush timeouts.
//
//  Parameters:
//      streamHandle     - The stream handle to disable
//      flushTimeoutMsec - If flushStream is non-zero, this is the maximum amount
//                         of time, in milliseconds, that we will wait for the
//                         writer device to flush.  If the timeout expires,
//                         we will immediately disable the stream.  If this value
//                         is -1, we will wait indefinitely.  If flushStream
//                         is zero, this parameter is ignored.
//      flushTimedOut    - Upon return, indicates whether the flush operation
//                         timed out (non-zero indicates timeout; zero indicates
//                         no timeout).  This parameter is ignored if it is NULL.
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pFlushAndDisableStream(nip2p_tStreamHandle streamHandle,
                                      int32_t flushTimeoutMsec,
                                      uint8_t* flushTimedOut);


//////////////////////////////////////////////////////////////////////////////
//
//  nip2pUnlinkStream
//
//  Description:
//      Unlinks the stream session identified by the given streamHandle.  If
//      the stream is currently enabled, it will automatically be disabled first
//      before unlinking the stream.  If the stream is already unlinked, this
//      function does nothing.
//
//  Parameters:
//      streamHandle - The stream handle to unlink
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pUnlinkStream(nip2p_tStreamHandle streamHandle);

//////////////////////////////////////////////////////////////////////////////
//
//  nip2pWaitForStreamEvent
//
//  Description:
//      Synchronously waits for the specified stream event.  This function does
//      not return until either (a) the specified stream event occurs, or (b)
//      the wait operation times out.  The maximum amount of time that we wait
//      is given by the timoutMsec parameter.  Depending upon the event, this
//      wait may be treated as a level-sensitive wait operation or as an
//      edge-sensitive wait.
//
//  Parameters:
//      streamHandle - The stream handle.
//      event        - The stream event that we are to wait for.
//      timeoutMsec  - The maximum amount of time to wait, in milliseconds.
//                     If this parameter is -1, we will wait indefinitely.
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pWaitForStreamEvent(nip2p_tStreamHandle streamHandle,
                                   nip2p_tStreamEvent event,
                                   int32_t timeoutMsec);

//////////////////////////////////////////////////////////////////////////////
//
//  nip2pGetAttribute
//
//  Description:
//      Queries the value of a stream attribute.
//
//  Parameters:
//      streamHandle - The stream handle.
//      attributeKey - Identifies the attribute whose value we want to query.
//      value        - Points to where the attribute value should be stored
//                     when we query it.  The data type of the object pointed
//                     to by this parameter must match the data type of the
//                     attribute we're querying.
//
//  Return Value:
//      Error code.
//
//////////////////////////////////////////////////////////////////////////////
NIP2P_FUNC nip2pGetAttribute(nip2p_tStreamHandle streamHandle,
                             nip2p_tStreamAttribute attributeKey,
                             void* value);

//#include "nistreamCommon/postfix.h"

#endif
