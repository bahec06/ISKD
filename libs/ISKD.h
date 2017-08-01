/*
 * Generated with the FPGA Interface C API Generator 14.0.0
 * for NI-RIO 14.0.0 or later.
 */

#ifndef __NiFpga_fpga_main_h__
#define __NiFpga_fpga_main_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1400
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_fpga_main_Bitfile;
 */
#define NiFpga_fpga_main_Bitfile "NiFpga_fpga_main.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_fpga_main_Signature = "CD1BD265572963CCBA30B3A8BE96418A";

typedef enum
{
   NiFpga_fpga_main_IndicatorBool_ch0_valid = 0x22,
   NiFpga_fpga_main_IndicatorBool_fir_valid = 0x1A,
   NiFpga_fpga_main_IndicatorBool_host_valid = 0xE,
   NiFpga_fpga_main_IndicatorBool_pnum_valid = 0x12,
   NiFpga_fpga_main_IndicatorBool_raw_signal_valid = 0x1E,
   NiFpga_fpga_main_IndicatorBool_w_valid = 0x16,
} NiFpga_fpga_main_IndicatorBool;

typedef enum
{
   NiFpga_fpga_main_IndicatorU16_state = 0x6,
} NiFpga_fpga_main_IndicatorU16;

typedef enum
{
   NiFpga_fpga_main_ControlBool_Play = 0x36,
   NiFpga_fpga_main_ControlBool_configure_form = 0x2,
   NiFpga_fpga_main_ControlBool_configure_spectrum = 0xA,
   NiFpga_fpga_main_ControlBool_random = 0x26,
   NiFpga_fpga_main_ControlBool_rms_on = 0x2E,
} NiFpga_fpga_main_ControlBool;

typedef enum
{
   NiFpga_fpga_main_ControlU16_rms = 0x2A,
} NiFpga_fpga_main_ControlU16;

typedef enum
{
   NiFpga_fpga_main_ControlU64_pulse_coeff = 0x30,
} NiFpga_fpga_main_ControlU64;

typedef enum
{
   NiFpga_fpga_main_HostToTargetFifoI16_host_pnum_fifo = 1,
} NiFpga_fpga_main_HostToTargetFifoI16;

typedef enum
{
   NiFpga_fpga_main_HostToTargetFifoU16_config_fifo = 2,
} NiFpga_fpga_main_HostToTargetFifoU16;

typedef enum
{
   NiFpga_fpga_main_PeerToPeerWriterFifoU32_p2p_fifo = 0,
} NiFpga_fpga_main_PeerToPeerWriterFifoU32;

#endif
