---=================
-- Uses SPI at 100 KHZ to communicate with PS controller
-- Commands are transferred to PS controller at the falling edge of the clock
-- Data is read at the rising edge of the clock
-- 6 bytes in total are transferred per sequence. The last 3 bytes ised for the button information
-- Buttons are ACTIVE LOW
-- Detailed information on PSX controller pins is listed here: http://pinouts.ru/Game/playstation_9_pinout.shtml
-- This code is written for a PS ONE controller so some differences for cycles and data from link
-- Controller requires Pull-up Resistors to 5V on data and ack line(if ack is used)
--=====================

library IEEE;
use IEEE.STD_LOGIC_1164.all;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

entity psController is
port(	
		CLOCK_50 : in std_logic;
		GPIO_0 : inout std_logic_vector(4 downto 0);
		psValueOut: out std_logic_vector(13 downto 0) --MS nibble Y coord, LS nibble X coord
		);

end psController;

architecture rtl of psController is
	signal clk_100KHZ : std_logic;
	signal clk_15KHZ : std_logic;
	signal att : std_logic;
	signal command: std_logic;
	signal data: std_logic;
	signal ack : std_logic;
	signal dataRegister : std_logic_vector(7 downto 0);
	
	signal psValue0: std_logic_vector(7 downto 0);
	signal psValue1: std_logic_vector(7 downto 0);
	signal psValue2: std_logic_vector(7 downto 0);


	constant c0 : std_logic_vector(7 downto 0) := x"01";
	constant c1 : std_logic_vector(7 downto 0) := x"42";
	signal states : std_logic_vector(3 downto 0);
	signal done : std_logic;
	constant dLEFT: std_logic_vector(2 downto 0) := "010";
	constant dRight: std_logic_vector(2 downto 0) := "001";
	constant dUp:  std_logic_vector(2 downto 0) := "100";
	constant dDown: std_logic_vector(2 downto 0) := "011";
	
begin

	data <= gpIO_0(0);
	gpIo_0(1) <= command;	
	gpIo_0(2) <= att;
	gpIo_0(3) <= clk_100KHZ;
	ack <= gpIO_0(4); -- ack is not used in our case

	

	
-------------
---Clock Divider making clk_100KHZ clock 
-------------
process(CLOCK_50)
	variable counter: std_logic_vector(8 downto 0);
begin
	if(rising_edge(clock_50)) then
			counter := counter + 1;
			if(counter = 0) then
				clk_100KHZ <= not clk_100KHZ;
			end if;
	end if;
end process;

-------------
---Clock Divider making 15K HZ clock **not used anymore
-------------
process(CLOCK_50)
	variable counter: std_logic_vector(19 downto 0);
begin
	if(rising_edge(clock_50)) then
			counter := counter + 1;
			if(counter = x"000000") then
				clk_15KHZ <= not clk_15KHZ;
			end if;
	end if;
end process;
-------------

--State machine at 100 KHZ
--Puts data onto output ports to PS controller on falling edge
--6 bytes of data transferred in total
------------
process(clk_100KHZ)
	variable index: integer range 0 to 9;
begin
	if(falling_edge(clk_100KHZ)) then
		case states is  
			when x"0" =>	-- initial delay to read in new commands		
				att <= '1'; 
				command <= '0';
				done <= '0';

				index := index + 1; --delay counter
				if(index = 8) then
					index := 0;
					states <= x"1";
				end if;
			when x"1" => -- Transmit first command: byte 1
				ATT <= '0'; --Pull low to signal start
				done <= '0';
				command <= c0(index); --shift one bit per cycle from LSB
				index := index + 1;
				if(index = 8) then
					index := 0;
					states <= x"2";
				end if;
			when x"2" => -- byte 2
				ATT <= '0';
				done <= '0';
				command <= c1(index);
				index := index + 1;
				if(index = 8) then
					index := 0;
					states <= x"3";
				end if;
			when x"3" => --byte 3
				ATT <= '0';
				done <= '0';
				command <= '0';
				index := index + 1;
				if(index = 8) then
					index := 0;
					states <= x"4";
				end if;
			when x"4" => --byte 4
				ATT <= '0';
				command <= '0';
				done <= '0';
				index := index + 1;
				if(index = 8) then
					index := 0;
					states <= x"5";
				end if;
			when x"5" => -- byte 5
				ATT <= '0';
				command <= '0';
				done <= '0';
				index := index + 1;
				if(index = 8) then
					index := 0;
					states <= x"6";
				end if;
			when x"6" => -- byte 6
				ATT <= '0';
				command <= '0';
				done <= '0';
				index := index + 1;
				if(index = 8) then
					index := 0;
					states <= x"7";
				end if;
			when others => -- Pull attention high to signal done
				states <= x"1";
				done <= '1';
				command <= '0';
				att <= '1';
			end case;
	end if;
end process;


------------
--State machine clk_100KHZ KHZ that reads data on the rising edge
--of the clock. Shifts take onto dataregister
-----------

process(clk_100KHZ)
	variable index: integer range 0 to 9;
begin

	if(rising_edge(clk_100KHZ)) then
		case states is
			when x"0" =>
			when x"1" =>
				index := index + 1;
				if(index = 8) then
					index := 0;
				end if;
			when x"2" =>
				
				index := index + 1;
				if(index = 8) then
					index := 0;		
				end if;
			when x"3" => --state for Circle
				index := index + 1;
				if(index = 8) then
					index := 0;
				end if;
			when x"4" => -- state for arrows, start, select
				dataRegister(index) <= NOT data; --shift bits onto dataRegister
				index := index + 1;
				if(index = 8) then
					index := 0;
					psValue0 <= dataRegister;
				end if;
			when x"5" => -- state for left and down arrow,  shapes, L and R buttons
				dataRegister(index) <= NOT data;
				index := index + 1;
				if(index = 8) then
					index := 0;
					psValue1 <= dataRegister;
				end if;
			when x"6" => -- state for square
				dataRegister(index) <= NOT data;
				index := index + 1;
				if(index = 8) then
					index := 0;
					psValue2 <= dataRegister;
				end if;
			when others =>
				--do nothing
			end case;
	end if;
end process;


-----------------------
--Process to determine to new direction for snake to go to 
--Snake cannot reverse into its own body
--Direction only changes on rising edge and after all bytes from PS controller 
--have been processed
--Slower clock to prevent key changes too fast and reversing direction
----------------------
process(clk_100KHZ) 

begin
	if(rising_edge(clk_100KHZ)) then
		if(done <= '1') then
			psValueOut(0) <= psValue0(1);
			psValueOut(1) <= psValue0(4);
			psValueOut(2) <= psValue0(5);
			psValueOut(3) <= psValue1(7);
			psValueOut(4) <= psValue1(0);
			psValueOut(5) <= psValue1(1);
			psValueOut(6) <= psValue1(2);
			psValueOut(7) <= psValue1(3);
			psValueOut(8) <= psValue1(4);
			psValueOut(9) <= psValue1(5);
			psValueOut(10) <= psValue0(6);
			psValueOut(11) <= psValue2(7);
			psValueOut(12) <= psValue2(0);
			psValueOut(13) <= psValue1(6);
		end if;
	end if;
end process;

end rtl;
