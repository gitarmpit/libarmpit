using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

internal class Program
{
    static void Main(string[] args)
    {
        string portName = "COM7";
        using (SerialPort serialPort = new SerialPort(portName))
        {
            try
            {
                serialPort.BaudRate = 300;
                serialPort.DataBits = 8;
                serialPort.StopBits = StopBits.One;
                serialPort.Parity = Parity.None;

                serialPort.Open();
                Console.WriteLine("Serial port opened.");

                byte b = 0; // Example byte
                while (true)
                {
                    Console.WriteLine("Sending: " + b);
                    WriteByte(serialPort, b++);
                    System.Threading.Thread.Sleep(1000);
                }

            }
            catch (UnauthorizedAccessException)
            {
                Console.WriteLine($"Access to {portName} denied.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
            finally
            {
                if (serialPort.IsOpen)
                {
                    serialPort.Close();
                    Console.WriteLine("Serial port closed.");
                }
            }

        }
    }
    static void WriteByte(SerialPort serialPort, byte data)
    {
        try
        {
            serialPort.Write(new byte[] { data }, 0, 1);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error writing byte to serial port: {ex.Message}");
        }
    }

}
