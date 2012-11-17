-------------------------------------------------------------------------------
-- $Id: baudrate.vhd,v 1.8 2007/01/29 07:33:48 nitink Exp $
-------------------------------------------------------------------------------
-- baudrate.vhd - entity/architecture pair 
-------------------------------------------------------------------------------
--
--  ***************************************************************************
--  **  Copyright(C) 2003, 2007 by Xilinx, Inc. All rights reserved.         **
--  **                                                                       **
--  **  This text contains proprietary, confidential                         **
--  **  information of Xilinx, Inc. , is distributed by                      **
--  **  under license from Xilinx, Inc., and may be used,                    **
--  **  copied and/or disclosed only pursuant to the terms                   **
--  **  of a valid license agreement with Xilinx, Inc.                       **
--  **                                                                       **
--  **  Unmodified source code is guaranteed to place and route,             **
--  **  function and run at speed according to the datasheet                 **
--  **  specification. Source code is provided "as-is", with no              **
--  **  obligation on the part of Xilinx to provide support.                 **
--  **                                                                       **
--  **  Xilinx Hotline support of source code IP shall only include          **
--  **  standard level Xilinx Hotline support, and will only address         **
--  **  issues and questions related to the standard released Netlist        **
--  **  version of the core (and thus indirectly, the original core source). **
--  **                                                                       **
--  **  The Xilinx Support Hotline does not have access to source            **
--  **  code and therefore cannot answer specific questions related          **
--  **  to source HDL. The Xilinx Support Hotline will only be able          **
--  **  to confirm the problem in the Netlist version of the core.           **
--  **                                                                       **
--  **  This copyright and support notice must be retained as part           **
--  **  of this text at all times.                                           **
--  ***************************************************************************
--
-------------------------------------------------------------------------------
-- Filename:        baudrate.vhd
-- Version:         v1.00a
-- Description:     Baud rate enable logic
--
-- VHDL-Standard:   VHDL'93
-------------------------------------------------------------------------------
-- Structure:   This section shows the hierarchical structure of xps_uartlite.
--
--              xps_uartlite.vhd
--                 --plbv46_slave_single.vhd
--                 --uartlite_core.vhd
--                    --uartlite_tx.vhd
--                    --uartlite_rx.vhd
--                    --baudrate.vhd
-------------------------------------------------------------------------------
-- Author:          MZC
--
-- History:
--  MZC     11/17/06
-- ^^^^^^
--  - Initial release of v1.00.a
-- ~~~~~~
--  NSK     01/24/07
-- ^^^^^^
-- Checking-in FLO modified files.
-- ~~~~~~
--  NSK     01/25/07
-- ^^^^^^
-- Code clean up.
-- ~~~~~~
--  NSK     01/29/07
-- ^^^^^^
-- Removed End of file statement.
-- ~~~~~~
-------------------------------------------------------------------------------
-- Naming Conventions:
--      active low signals:                     "*_n"
--      clock signals:                          "clk", "clk_div#", "clk_#x"
--      reset signals:                          "rst", "rst_n"
--      generics:                               "C_*"
--      user defined types:                     "*_TYPE"
--      state machine next state:               "*_ns"
--      state machine current state:            "*_cs"
--      combinatorial signals:                  "*_com"
--      pipelined or register delay signals:    "*_d#"
--      counter signals:                        "*cnt*"
--      clock enable signals:                   "*_ce"
--      internal version of output port         "*_i"
--      device pins:                            "*_pin"
--      ports:                                  - Names begin with Uppercase
--      processes:                              "*_PROCESS"
--      component instantiations:               "<ENTITY_>I_<#|FUNC>
-------------------------------------------------------------------------------
library IEEE;
use IEEE.std_logic_1164.all;

-------------------------------------------------------------------------------
-- Port Declaration
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- Definition of Generics :
-------------------------------------------------------------------------------
-- UART Lite generics
--  C_RATIO               -- The ratio between clk and the asked baudrate
--                           multiplied with 16
--  C_INACCURACY          -- The maximum inaccuracy of the clk division in per 
--                           thousands
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- Definition of Ports :
-------------------------------------------------------------------------------
-- System Signals
--  Clk                   --  Clock signal
-- Internal UART interface signals
--  EN_16x_Baud           --  Enable signal which is 16x times baud rate
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--                  Entity Section
-------------------------------------------------------------------------------
entity baudrate is
  generic
   (
    C_RATIO      : integer := 48;  -- The ratio between clk and the asked
                                   -- baudrate multiplied with 16
    C_INACCURACY : integer := 20   -- The maximum inaccuracy of the clk
   );                              -- division in per thousands
  port
   (
    Clk          : in  std_logic;
    EN_16x_Baud  : out std_logic
   );
end entity baudrate;

-------------------------------------------------------------------------------
-- Architecture Section
-------------------------------------------------------------------------------
architecture imp of baudrate is

    ---------------------------------------------------------------------------
    -- Signal Declarations
    ---------------------------------------------------------------------------
    signal Count : natural range 0 to C_RATIO-1;

begin  -- architecture VHDL_RTL

    ---------------------------------------------------------------------------
    -- COUNTER_PROCESS : Down counter for generating EN_16x_Baud signal
    ---------------------------------------------------------------------------
    COUNTER_PROCESS : process (Clk) is
        begin
            if Clk'event and Clk = '1' then  -- rising clock edge
                if (Count = 0) then
                    Count       <= C_RATIO-1;
                    EN_16x_Baud <= '1';
                else
                    Count       <= Count - 1;
                    EN_16x_Baud <= '0';
                end if;
            end if;
    end process COUNTER_PROCESS;

end architecture imp;
