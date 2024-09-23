import serial
import serial.tools.list_ports
import time
import threading

class SerialLogic:
    def __init__(self):
        self.port_pairs = {}
        self.stop_event = threading.Event()
        self.read_threads = []
        self.port_speeds = {}
        self.baudrate = 9600  # Скорость порта по умолчанию

    def get_port_speed(self, port):
        return self.baudrate

    def populate_ports(self):
        ports = serial.tools.list_ports.comports()
        self.ports_list = [port.device for port in ports if port.device not in ["COM6", "COM7"]]

        if len(self.ports_list) < 4:
            raise ValueError("Недостаточно доступных COM-портов.")

        self.create_port_pairs()
        return self.ports_list

    def create_port_pairs(self):
        checked_ports = set()

        for i in range(len(self.ports_list)):
            port1 = self.ports_list[i]
            if port1 in checked_ports:
                continue

            for j in range(i + 1, len(self.ports_list)):
                port2 = self.ports_list[j]
                if port2 in checked_ports:
                    continue

                if self.test_connection(port1, port2):
                    self.port_pairs[port1] = port2
                    self.port_pairs[port2] = port1
                    checked_ports.add(port1)
                    checked_ports.add(port2)
                    print(f"Связь установлена: {port1} <-> {port2}")
                    break

    def test_connection(self, port1, port2):
        print(f"Тестируем соединение между {port1} и {port2}...")
        try:
            with serial.Serial(port1, self.baudrate, timeout=1) as ser1, serial.Serial(port2, self.baudrate, timeout=1) as ser2:
                #time.sleep(2)
                test_message = "TEST"
                ser1.write(test_message.encode())
                #time.sleep(1)

                if ser2.in_waiting > 0:
                    response = ser2.read(ser2.in_waiting).decode()
                    print(f"Получено сообщение: {response} от {port2}")
                    return response == test_message
                else:
                    print("Нет ответа от порта.")
                    return False
        except serial.SerialException as e:
            print(f"Ошибка: {e}")
            return False

    def send_packet(self, send_port, receive_port, message, parity):
        parity_setting = {
            "None": serial.PARITY_NONE,
            "Even": serial.PARITY_EVEN,
            "Odd": serial.PARITY_ODD
        }[parity]

        self.port_speeds[send_port] = self.baudrate
        self.port_speeds[receive_port] = self.baudrate

        try:
            with serial.Serial(send_port, self.baudrate, timeout=1, parity=parity_setting) as ser1, \
                 serial.Serial(receive_port, self.baudrate, timeout=1, parity=parity_setting) as ser2:
                time.sleep(1)  # Задержка для установления соединения
                ser1.write(message.encode())

                # Проверка ответа
                if ser2.in_waiting > 0:
                    response = ser2.read(ser2.in_waiting).decode()
                    print(f"-> {response}")
                    return response
                else:
                    print("Нет ответа от порта.")
                    return None
        except serial.SerialException as e:
            print(f"Ошибка: {e}")
            return None
        
    def bit_stuffing(self, data):
        stuffed = ''
        count = 0
        for bit in data:
            stuffed += bit
            if bit == '1':
                count += 1
                if count == 5:
                    stuffed += '0'  # Вставляем 0 после пяти единиц
                    count = 0
            else:
                count = 0
        return stuffed
    
    def highlight_bit_stuffing(self, data):
        highlighted_data = ''
        count = 0
        
        for i in range(len(data)):
            bit = data[i]
            highlighted_data += bit
            
            if bit == '1':
                count += 1
            else:
                count = 0
            
            # Проверка на наличие 5 единиц подряд
            if count == 5 and i + 1 < len(data) and data[i + 1] == '0':
                highlighted_data += '_'  # Добавляем кавычки перед вставленным битом
                highlighted_data += '0'    # Вставляем бит-стафф
                highlighted_data += '_'    # Добавляем кавычки после вставленного бита
                count = 0  # Сброс счетчика

        return highlighted_data

    def de_bit_stuffing(self, stuffed_data):
        unstuffed = ''
        count = 0
        for bit in stuffed_data:
            unstuffed += bit
            if bit == '1':
                count += 1
                if count == 5:
                    # Пропускаем следующий бит, если это 0 (бит-стаффинг)
                    continue
            else:
                count = 0
        return unstuffed