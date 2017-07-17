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
static const char* const NiFpga_fpga_main_Signature = "01EBFC66A1B4A73B8ABA9D2E0E92D804";

typedef enum
{
   NiFpga_fpga_main_IndicatorBool_f_conf = 0xE,
   NiFpga_fpga_main_IndicatorBool_s_conf = 0x22,
} NiFpga_fpga_main_IndicatorBool;

typedef enum
{
   NiFpga_fpga_main_IndicatorU64_frequency = 0x24,
} NiFpga_fpga_main_IndicatorU64;

typedef enum
{
   NiFpga_fpga_main_ControlBool_Const_current = 0x16,
   NiFpga_fpga_main_ControlBool_Noise = 0x5A,
   NiFpga_fpga_main_ControlBool_Play = 0x2E,
   NiFpga_fpga_main_ControlBool_Set_Wide = 0x5E,
   NiFpga_fpga_main_ControlBool_Two_channels = 0x1A,
   NiFpga_fpga_main_ControlBool_configure_form = 0x6,
   NiFpga_fpga_main_ControlBool_configure_spectrum = 0x2,
   NiFpga_fpga_main_ControlBool_stop_vi = 0xA,
} NiFpga_fpga_main_ControlBool;

typedef enum
{
   NiFpga_fpga_main_ControlI16_Threshold = 0x2A,
} NiFpga_fpga_main_ControlI16;

typedef enum
{
   NiFpga_fpga_main_ControlU16_RMS = 0x36,
   NiFpga_fpga_main_ControlU16_TMode = 0x32,
   NiFpga_fpga_main_ControlU16_WideRMS = 0x1E,
} NiFpga_fpga_main_ControlU16;

typedef enum
{
   NiFpga_fpga_main_ControlU32_Time = 0x54,
} NiFpga_fpga_main_ControlU32;

typedef enum
{
   NiFpga_fpga_main_ControlI64_K0 = 0x50,
   NiFpga_fpga_main_ControlI64_K1 = 0x4C,
   NiFpga_fpga_main_ControlI64_K2 = 0x44,
   NiFpga_fpga_main_ControlI64_K3 = 0x48,
} NiFpga_fpga_main_ControlI64;

typedef enum
{
   NiFpga_fpga_main_ControlU64_C1 = 0x38,
   NiFpga_fpga_main_ControlU64_Const_freq = 0x10,
   NiFpga_fpga_main_ControlU64_Offset = 0x40,
   NiFpga_fpga_main_ControlU64_Pulse_Coeff = 0x3C,
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
