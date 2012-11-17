-------------------------------------------------------------------------------
-- $Id: uartlite_tx.vhd,v 1.10 2007/02/09 08:42:41 nitink Exp $
-------------------------------------------------------------------------------
-- uartlite_tx.vhd - entity/architecture pair
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
-- Filename:        uartlite_tx.vhd
-- Version:         v1.00a
-- Description:     UART Lite Transmit Interface Module
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
-- 1. Code clean up.
-- 2. Added FIFO write/read inhibit when FIFO is FULL/EMPTY.
-- ~~~~~~
--  NSK     01/29/07
-- ^^^^^^
-- 1. Removed End of file statement.
-- 2. Added internal signal tx_buffer_full_i for reading output port 
--    TX_Buffer_Full (xst error).
-- ~~~~~~
--  NSK     02/09/07
-- ^^^^^^
-- All asynchronous reset is changed to synchronous reset.
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
use IEEE.numeric_std.UNSIGNED;
use IEEE.numeric_std.to_unsigned;
use IEEE.numeric_std."-";

library proc_common_v2_00_a;
-- dynshreg_i_f refered from proc_common_v2_00_a
use proc_common_v2_00_a.dynshreg_i_f;
-- srl_fifo_f refered from proc_common_v2_00_a
use proc_common_v2_00_a.srl_fifo_f;

-------------------------------------------------------------------------------
-- Port Declaration
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- Definition of Generics :
-------------------------------------------------------------------------------
-- UART Lite generics
--  C_DATA_BITS     -- The number of data bits in the serial frame
--  C_USE_PARITY    -- Determines whether parity is used or not
--  C_ODD_PARITY    -- If parity is used determines whether parity
--                     is even or odd
-- PLBv46 Slave Single block generics
--  C_FAMILY        -- Xilinx FPGA Family
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- Definition of Ports :
-------------------------------------------------------------------------------
-- System Signals
--  Clk               --  Clock signal
--  Rst               --  Reset signal
-- UART Lite interface
--  TX                --  Transmit Data
--  TX_Active         --  Transmit in Progress
-- Internal UART interface signals
--  EN_16x_Baud       --  Enable signal which is 16x times baud rate
--  Write_TX_FIFO     --  Write transmit FIFO
--  Reset_TX_FIFO     --  Reset transmit FIFO
--  TX_Data           --  Transmit data input
--  TX_Buffer_Full    --  Transmit buffer full
--  TX_Buffer_Empty   --  Transmit buffer empty

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--                  Entity Section
-------------------------------------------------------------------------------
entity uartlite_tx is
  generic 
   (
    C_DATA_BITS     : integer range 5 to 8 := 8;
    C_USE_PARITY    : integer range 0 to 1 := 1;
    C_ODD_PARITY    : integer range 0 to 1 := 1;
    C_FAMILY        : string               := "virtex5"
   );
  port
   (
    Clk             : in  std_logic;
    Reset           : in  std_logic;
    EN_16x_Baud     : in  std_logic;
    TX              : out std_logic;
    TX_Active       : out std_logic;
    Write_TX_FIFO   : in  std_logic;
    Reset_TX_FIFO   : in  std_logic;
    TX_Data         : in  std_logic_vector(0 to C_DATA_BITS-1);
    TX_Buffer_Full  : out std_logic;
    TX_Buffer_Empty : out std_logic
   );
end entity uartlite_tx;

-------------------------------------------------------------------------------
-- Architecture Section
-------------------------------------------------------------------------------
architecture imp of uartlite_tx is

    type     bo2sl_type is array(boolean) of std_logic;
    constant bo2sl      :  bo2sl_type := (false => '0', true => '1');

    -------------------------------------------------------------------------
    --  Constant Declarations
    -------------------------------------------------------------------------
    constant mux_sel_init : std_logic_vector(0 to 2) :=
      std_logic_vector(to_unsigned(C_DATA_BITS-1, 3));

    -------------------------------------------------------------------------
    -- Signal Declarations
    -------------------------------------------------------------------------
    signal parity            : std_logic;
    signal tx_Run1           : std_logic;
    signal select_Parity     : std_logic;
    signal data_to_transfer  : std_logic_vector(0 to C_DATA_BITS-1);
    signal div16             : std_logic_vector(0 to 0);
    signal tx_Data_Enable    : std_logic;
    signal tx_Start          : std_logic;
    signal tx_DataBits       : std_logic;
    signal tx_Run            : std_logic;
    signal mux_sel           : std_logic_vector(0 to 2);
    signal mux_sel_is_zero   : std_logic;
    signal mux_01            : std_logic;
    signal mux_23            : std_logic;
    signal mux_45            : std_logic;
    signal mux_67            : std_logic;
    signal mux_0123          : std_logic;
    signal mux_4567          : std_logic;
    signal mux_Out           : std_logic;
    signal serial_Data       : std_logic;
    signal fifo_Read         : std_logic;
    signal fifo_Data_Present : std_logic := '0';
    signal fifo_Data_Empty   : std_logic;
    signal fifo_DOut         : std_logic_vector(0 to C_DATA_BITS-1);
    signal fifo_wr           : std_logic;
    signal fifo_rd           : std_logic;
    signal tx_buffer_full_i  : std_logic;

begin  -- architecture IMP

    ---------------------------------------------------------------------------
    --DIV16_SRL16E_I : Divide the EN_16x_Baud by 16 to get the correct baudrate
    ---------------------------------------------------------------------------
    MID_START_BIT_SRL16_I : entity proc_common_v2_00_a.dynshreg_i_f
      generic map
       (
        C_DEPTH      => 16,
        C_DWIDTH     => 1,
        C_INIT_VALUE => X"8000",
        C_FAMILY     => C_FAMILY
       )
      port map
       (
        Clk   => Clk,
        Clken => EN_16x_Baud,
        Addr  => "1111",
        Din   => div16,
        Dout  => div16
       );

      TX_DATA_ENABLE_DFF: Process (Clk) is
      begin
          if (Clk'event and Clk = '1') then
              if (tx_Data_Enable = '1') then
                  tx_Data_Enable <= '0';
              elsif (EN_16x_Baud = '1') then
                  tx_Data_Enable <= div16(0);
              end if;
          end if;
      end process TX_DATA_ENABLE_DFF;

    ------------------------------------------------------------------------
    -- TX_START_DFF : tx_start is '1' for the start bit in a transmission
    ------------------------------------------------------------------------
    TX_START_DFF : process (Clk) is
    begin
        if Clk'event and Clk = '1' then -- rising clock edge
            if Reset = '1' then         -- synchronous reset (active high)
                tx_Start <= '0';
            else 
                tx_Start <= not(tx_Run) and (tx_Start or 
                               (fifo_Data_Present and tx_Data_Enable));
            end if;
        end if;
    end process TX_START_DFF;

    --------------------------------------------------------------------------
    -- TX_DATA_DFF : tx_DataBits is '1' during all databits transmission
    --------------------------------------------------------------------------
    TX_DATA_DFF : process (Clk) is
    begin
        if Clk'event and Clk = '1' then -- rising clock edge
            if Reset = '1' then         -- synchronous reset (active high)
                tx_DataBits <= '0';
            else 
                tx_DataBits <= not(fifo_Read) and (tx_DataBits or 
                                           (tx_Start and tx_Data_Enable));
            end if;
        end if;
    end process TX_DATA_DFF;

    -------------------------------------------------------------------------
    -- COUNTER : If mux_sel is zero then reload with the init value else if 
    --           tx_DataBits = '1', decrement
    -------------------------------------------------------------------------
    COUNTER : process (Clk, Reset) is
    begin  -- process Mux_Addr_DFF
        if Clk'event and Clk = '1' then -- rising clock edge
            if Reset = '1' then         -- synchronous reset (active high)
                mux_sel <= std_logic_vector(to_unsigned(C_DATA_BITS-1, mux_sel'length));
            elsif (tx_Data_Enable = '1') then
                if (mux_sel_is_zero = '1') then
                    mux_sel <= mux_sel_init;
                elsif (tx_DataBits = '1') then
                    mux_sel <= std_logic_vector(UNSIGNED(mux_sel) - 1);
                end if;
            end if;
        end if;
    end process COUNTER;

    ------------------------------------------------------------------------
    -- Detecting when mux_sel is zero, i.e. all data bits are transfered
    ------------------------------------------------------------------------
    mux_sel_is_zero <= '1' when mux_sel = "000" else '0';

    --------------------------------------------------------------------------
    -- FIFO_READ_DFF : Read out the next data from the transmit fifo when the 
    --                 data has been transmitted
    --------------------------------------------------------------------------
    FIFO_READ_DFF : process (Clk) is
    begin  -- process FIFO_Read_DFF
        if Clk'event and Clk = '1' then -- rising clock edge
            if Reset = '1' then         -- synchronous reset (active high)
                fifo_Read <= '0';
            else
                fifo_Read <= tx_Data_Enable and mux_sel_is_zero;
            end if;
        end if;
    end process FIFO_READ_DFF;
    --------------------------------------------------------------------------
    -- Select which bit within the data word to transmit
    --------------------------------------------------------------------------

    --------------------------------------------------------------------------
    -- PARITY_BIT_INSERTION : Need special treatment for inserting the parity 
    --                        bit because of parity generation
    --------------------------------------------------------------------------
    PARITY_BIT_INSERTION : process (parity, select_Parity, fifo_DOut) is
    begin  -- process Parity_Bit_Insertion
        data_to_transfer <= fifo_DOut;
        if (select_Parity = '1') then
            data_to_transfer(C_DATA_BITS-1) <= parity;
        end if;
    end process PARITY_BIT_INSERTION;

    --data_to_transfer(C_DATA_BITS-1) <= parity when select_Parity = '1' else
    --                                   fifo_DOut;

    mux_01 <= data_to_transfer(1) when mux_sel(2) = '1' else 
              data_to_transfer(0);
    mux_23 <= data_to_transfer(3) when mux_sel(2) = '1' else
              data_to_transfer(2);

    --------------------------------------------------------------------------
    -- Data_Bits_Is_5 : Select total 5 data bits when C_DATA_BITS = 5
    --------------------------------------------------------------------------
    Data_Bits_Is_5 : if (C_DATA_BITS = 5) generate
        mux_45 <= data_to_transfer(4);
        mux_67 <= '0';
    end generate Data_Bits_Is_5;

    --------------------------------------------------------------------------
    -- Data_Bits_Is_6 : Select total 6 data bits when C_DATA_BITS = 6
    --------------------------------------------------------------------------
    Data_Bits_Is_6 : if (C_DATA_BITS = 6) generate
        mux_45 <= data_to_transfer(5) when mux_sel(2) = '1' else 
                  data_to_transfer(4);
        mux_67 <= '0';
    end generate Data_Bits_Is_6;

    --------------------------------------------------------------------------
    -- Data_Bits_Is_7 : Select total 7 data bits when C_DATA_BITS = 7
    --------------------------------------------------------------------------
    Data_Bits_Is_7 : if (C_DATA_BITS = 7) generate
        mux_45 <= data_to_transfer(5) when mux_sel(2) = '1' else 
                  data_to_transfer(4);
        mux_67 <= data_to_transfer(6);
    end generate Data_Bits_Is_7;

    --------------------------------------------------------------------------
    -- Data_Bits_Is_8 : Select total 8 data bits when C_DATA_BITS = 8
    --------------------------------------------------------------------------
    Data_Bits_Is_8 : if (C_DATA_BITS = 8) generate
      mux_45 <= data_to_transfer(5) when mux_sel(2) = '1' else 
                data_to_transfer(4);
      mux_67 <= data_to_transfer(7) when mux_sel(2) = '1' else 
                data_to_transfer(6);
    end generate Data_Bits_Is_8;

    mux_0123 <= mux_23   when mux_sel(1) = '1' else mux_01;
    mux_4567 <= mux_67   when mux_sel(1) = '1' else mux_45;
    mux_out  <= mux_4567 when mux_sel(0) = '1' else mux_0123;

    SERIAL_DATA_DFF : process (Clk) is
    begin
        if Clk'event and Clk = '1' then  -- rising clock edge
            if Reset = '1' then          -- synchronous reset (active high)
                serial_Data <= '0';
            else 
                serial_Data <= mux_Out;
            end if;
        end if;
    end process SERIAL_DATA_DFF;

    --------------------------------------------------------------------------
    -- SERIAL_OUT_DFF :Force a '0' when tx_start is '1', Start_bit
    --                 Force a '1' when tx_Run is '0',   Idle
    --                 otherwise put out the serial_data
    --------------------------------------------------------------------------
    SERIAL_OUT_DFF : process (Clk) is
    begin  -- process Serial_Out_DFF
        if Clk'event and Clk = '1' then -- rising clock edge
            if Reset = '1' then         -- synchronous reset (active high)
                TX <= '1';
                TX_Active <= '0';
            else 
                TX <= (not(tx_Run) or serial_Data) and not(tx_Start);
		TX_Active <= tx_Run or tx_Start;
            end if;
        end if;
    end process SERIAL_OUT_DFF;

    --------------------------------------------------------------------------
    -- USING_PARITY : Generate parity handling when C_USE_PARITY = 1
    --------------------------------------------------------------------------
    USING_PARITY : if (C_USE_PARITY = 1) generate

        PARITY_DFF: Process (Clk) is
        begin
            if (Clk'event and Clk = '1') then
                if (tx_Start = '1') then
                    Parity <=  bo2sl(C_ODD_PARITY = 1);
                elsif (tx_Data_Enable = '1') then
                    Parity <= parity xor serial_data;
                end if;
            end if;
        end process PARITY_DFF;

        TX_RUN1_DFF : process (Clk) is
        begin
            if Clk'event and Clk = '1' then -- rising clock edge
                if Reset = '1' then         -- synchronous reset (active high)
                    tx_Run1 <= '0';
                elsif (tx_Data_Enable = '1') then
                    tx_Run1 <= tx_DataBits;
                end if;
            end if;
        end process TX_RUN1_DFF;

        tx_Run <= tx_Run1 or tx_DataBits;

        SELECT_PARITY_DFF : process (Clk) is
        begin
            if Clk'event and Clk = '1' then  -- rising clock edge
                if Reset = '1' then          -- synchronous reset (active high)
                    select_Parity <= '0';
                elsif (tx_Data_Enable = '1') then
                    select_Parity <= mux_sel_is_zero;
                end if;
            end if;
        end process SELECT_PARITY_DFF;

    end generate USING_PARITY;

    --------------------------------------------------------------------------
    -- NO_PARITY : When C_USE_PARITY = 0 select parity as '0'
    --------------------------------------------------------------------------
    NO_PARITY : if (C_USE_PARITY = 0) generate
        tx_Run        <= tx_DataBits;
        select_Parity <= '0';
    end generate NO_PARITY;

    --------------------------------------------------------------------------
    -- SRL_FIFO_I : Transmit FIFO Interface
    --------------------------------------------------------------------------
    fifo_wr <= Write_TX_FIFO and (not tx_buffer_full_i);
    fifo_rd <= fifo_Read and (not fifo_Data_Empty);

    SRL_FIFO_I : entity proc_common_v2_00_a.srl_fifo_f
      generic map 
       (
        C_DWIDTH   => C_DATA_BITS,
        C_DEPTH    => 16,
        C_FAMILY   => C_FAMILY
       )
      port map
       (
        Clk        => Clk,
        Reset      => Reset_TX_FIFO,
        FIFO_Write => fifo_wr,
        Data_In    => TX_Data,
        FIFO_Read  => fifo_rd,
        Data_Out   => fifo_DOut,
        FIFO_Full  => tx_buffer_full_i,
        FIFO_Empty => fifo_Data_Empty
       );

    TX_Buffer_Full    <= tx_buffer_full_i;
    TX_Buffer_Empty   <= fifo_Data_Empty;
    fifo_Data_Present <= not fifo_Data_Empty;

end architecture imp;
