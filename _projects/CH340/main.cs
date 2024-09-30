using System;
using System.IO.Ports;

class Program
{
    static void Main()
    {
        string portName = "COM7";
        using (SerialPort serialPort = new SerialPort(portName))
        {
            try
            {
                serialPort.BaudRate = 9600;
                serialPort.DataBits = 8;
                serialPort.StopBits = StopBits.One;
                serialPort.Parity = Parity.None;

                serialPort.Open();
                Console.WriteLine("Serial port opened.");

                byte byteToSend = 0x01; // Example byte
                WriteByte(serialPort, byteToSend);
                byte receivedByte = ReadByte(serialPort);
                Console.WriteLine($"Received byte: {receivedByte}");

                // Example: Write and read an array of bytes
                byte[] bytesToSend = { 0x02, 0x03, 0x04 };
                WriteBytes(serialPort, bytesToSend);
                byte[] receivedBytes = ReadBytes(serialPort, 3);
                Console.WriteLine($"Received bytes: {BitConverter.ToString(receivedBytes)}");
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
            Console.WriteLine($"Sent byte: {data}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error writing byte to serial port: {ex.Message}");
        }
    }

    static void WriteBytes(SerialPort serialPort, byte[] data)
    {
        try
        {
            serialPort.Write(data, 0, data.Length);
            Console.WriteLine($"Sent bytes: {BitConverter.ToString(data)}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error writing bytes to serial port: {ex.Message}");
        }
    }

    static byte ReadByte(SerialPort serialPort)
    {
        try
        {
            if (serialPort.BytesToRead > 0)
            {
                return (byte)serialPort.ReadByte();
            }
            else
            {
                Console.WriteLine("No data available to read.");
                return 0; // or throw an exception as needed
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error reading byte from serial port: {ex.Message}");
            return 0; // or throw an exception as needed
        }
    }

    static byte[] ReadBytes(SerialPort serialPort, int count)
    {
        try
        {
            if (serialPort.BytesToRead >= count)
            {
                byte[] buffer = new byte[count];
                serialPort.Read(buffer, 0, count);
                return buffer;
            }
            else
            {
                Console.WriteLine("Not enough data available to read.");
                return Array.Empty<byte>(); // return an empty array or handle as needed
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error reading bytes from serial port: {ex.Message}");
            return Array.Empty<byte>(); // return an empty array or handle as needed
        }
    }
}
