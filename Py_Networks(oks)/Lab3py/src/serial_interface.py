import tkinter as tk
from tkinter import scrolledtext, ttk, messagebox
from serial_logic import SerialLogic
from crc_code.systematic_cyclic_code import SystematicCyclicCode

class SerialInterfaceApp:

    FLAG_SIZE = 2
    flag = "$x" 

    DESTINATION_ADDRESS_SIZE = 1
    destination = bytes([0x30])

    SOURCE_ADDRESS_SIZE = 1
    DATA_SIZE = 24

    FCS_SIZE = 4

    def __init__(self, master):
        self.master = master
        self.master.title("Serial Interface")
        self.logic = SerialLogic()

        self.master.minsize(1500, 600)  

        self.setup_ui()
        self.populate_ports()

    def setup_ui(self):
        # Окно для выбора порта отправки
        self.send_frame = tk.Frame(self.master)
        self.send_frame.pack(pady=10)

        self.send_port_label = tk.Label(self.send_frame, text="Пара отправки:")
        self.send_port_label.pack(side=tk.LEFT, padx=5)

        self.send_port_combobox = ttk.Combobox(self.send_frame, width=15)
        self.send_port_combobox.pack(side=tk.LEFT, padx=5)

        self.send_connected_port_label = tk.Label(self.send_frame, text="Связанный порт:")
        self.send_connected_port_label.pack(side=tk.LEFT, padx=5)

        self.send_connected_port = tk.Label(self.send_frame, text="")
        self.send_connected_port.pack(side=tk.LEFT, padx=5)

        self.send_port_combobox.bind("<<ComboboxSelected>>", self.update_connected_send_port)

        # Окно для выбора порта чтения
        self.read_frame = tk.Frame(self.master)
        self.read_frame.pack(pady=10)

        self.read_port_label = tk.Label(self.read_frame, text="Пара чтения:")
        self.read_port_label.pack(side=tk.LEFT, padx=5)

        self.read_port_combobox = ttk.Combobox(self.read_frame, width=15)
        self.read_port_combobox.pack(side=tk.LEFT, padx=5)

        self.read_connected_port_label = tk.Label(self.read_frame, text="Связанный порт:")
        self.read_connected_port_label.pack(side=tk.LEFT, padx=5)

        self.read_connected_port = tk.Label(self.read_frame, text="")
        self.read_connected_port.pack(side=tk.LEFT, padx=5)

        self.read_port_combobox.bind("<<ComboboxSelected>>", self.update_connected_read_port)

        # Окно вывода
        self.output_frame = tk.Frame(self.master)
        self.output_frame.pack(pady=10, fill=tk.X)  # Заполнение по ширине

        self.left_output_frame = tk.Frame(self.output_frame)
        self.left_output_frame.pack(side=tk.LEFT, padx=10, fill=tk.BOTH, expand=True)  # Расширяем

        self.right_output_frame = tk.Frame(self.output_frame)
        self.right_output_frame.pack(side=tk.RIGHT, padx=10, fill=tk.BOTH, expand=True)  # Расширяем

        self.left_output_label = tk.Label(self.left_output_frame, text="Вывод с первого порта:")
        self.left_output_label.pack()

        self.left_output_text = scrolledtext.ScrolledText(self.left_output_frame, width=80, height=10, state='disabled')  # Увеличиваем ширину
        self.left_output_text.pack(padx=5, pady=5, fill=tk.BOTH, expand=True)  # Заполнение и расширение

        self.right_output_label = tk.Label(self.right_output_frame, text="Вывод со второго порта:")
        self.right_output_label.pack()

        self.right_output_text = scrolledtext.ScrolledText(self.right_output_frame, width=80, height=10, state='disabled')  # Увеличиваем ширину
        self.right_output_text.pack(padx=5, pady=5, fill=tk.BOTH, expand=True)  # Заполнение и расширение

        # Параметры паритета для первого порта 
        self.parity_frame_send = tk.Frame(self.master)
        self.parity_frame_send.pack(pady=10)

        self.parity_label_send = tk.Label(self.parity_frame_send, text="Паритет первого порта:")
        self.parity_label_send.pack(side=tk.LEFT, padx=5)

        self.parity_combobox_send = ttk.Combobox(self.parity_frame_send, values=["None", "Even", "Odd"], state='readonly', width=10)
        self.parity_combobox_send.current(0)  # По умолчанию None
        self.parity_combobox_send.pack(side=tk.LEFT, padx=5)

        # Окно ввода сообщения для первого порта 
        self.input_frame_send = tk.Frame(self.master)
        self.input_frame_send.pack(pady=10)

        self.input_label_send = tk.Label(self.input_frame_send, text="Ввод первого порта:")
        self.input_label_send.pack(side=tk.LEFT, padx=5)

        self.input_entry_send = tk.Entry(self.input_frame_send, width=40)
        self.input_entry_send.pack(side=tk.LEFT, padx=5)
        self.input_entry_send.bind("<Return>", self.send_message_first)

        # Параметры паритета для второго порта
        self.parity_frame_receive = tk.Frame(self.master)
        self.parity_frame_receive.pack(pady=10)

        self.parity_label_receive = tk.Label(self.parity_frame_receive, text="Паритет второго порта:")
        self.parity_label_receive.pack(side=tk.LEFT, padx=5)

        self.parity_combobox_receive = ttk.Combobox(self.parity_frame_receive, values=["None", "Even", "Odd"], state='readonly', width=10)
        self.parity_combobox_receive.current(0)  # По умолчанию None
        self.parity_combobox_receive.pack(side=tk.LEFT, padx=5)

        # Окно ввода сообщения для второго порта
        self.input_frame_receive = tk.Frame(self.master)
        self.input_frame_receive.pack(pady=10)

        self.input_label_receive = tk.Label(self.input_frame_receive, text="Ввод второго порта:")
        self.input_label_receive.pack(side=tk.LEFT, padx=5)

        self.input_entry_receive = tk.Entry(self.input_frame_receive, width=40)
        self.input_entry_receive.pack(side=tk.LEFT, padx=5)
        self.input_entry_receive.bind("<Return>", self.send_message_second)

        # Окно состояния
        self.status_frame = tk.Frame(self.master)
        self.status_frame.pack(pady=10)

        self.port_speed_label = tk.Label(self.status_frame, text="Скорость порта: 0")
        self.port_speed_label.pack(pady=5)

        self.bytes_sent_label = tk.Label(self.status_frame, text="Передано байт: 0")
        self.bytes_sent_label.pack(pady=5)
        
        # Новый элемент для отображения текущего кадра
        self.current_frame_label = tk.Label(self.status_frame, text="Текущий кадр: ")
        self.current_frame_label.pack(pady=5)

        self.bytes_sent = 0

    def update_status_window(self, port_speed, bytes_sent):
        self.port_speed_label.config(text=f"Скорость порта: {port_speed}")
        self.bytes_sent_label.config(text=f"Передано байт: {bytes_sent}")

    def populate_ports(self):
        try:
            ports = self.logic.populate_ports()
            self.send_port_combobox['values'] = ports
            self.read_port_combobox['values'] = ports

            if ports:
                self.send_port_combobox.current(0)
                self.read_port_combobox.current(1)

            self.update_connected_send_port()
            self.update_connected_read_port()
        except ValueError as e:
            messagebox.showerror("Ошибка", str(e))

    def update_connected_send_port(self, event=None):
        selected_port = self.send_port_combobox.get()
        connected_port = self.get_connected_port(selected_port)
        self.send_connected_port.config(text=connected_port)
        self.update_read_ports()

    def update_connected_read_port(self, event=None):
        selected_port = self.read_port_combobox.get()
        connected_port = self.get_connected_port(selected_port)
        self.read_connected_port.config(text=connected_port)

    def update_read_ports(self):
        selected_send_port = self.send_port_combobox.get()
        connected_send_port = self.get_connected_port(selected_send_port)

        available_ports = [
            port for port in self.logic.ports_list
            if port != selected_send_port and port != connected_send_port
        ]

        self.read_port_combobox['values'] = available_ports
        if available_ports:
            self.read_port_combobox.current(0)
        else:
            self.read_port_combobox.set('')


    def send_message(self, input_entry, output_text, port_combobox, is_first=True):
        message = input_entry.get().strip() 
        print(message)
        if not message:
            return

        selected_port = port_combobox.get()
        connected_port = self.get_connected_port(selected_port)

        if connected_port == "Нет связанного порта":
            messagebox.showerror("Ошибка", "Нет связанного порта для отправки.")
            return

        source_address = str(selected_port.replace('COM', ''))  # Номер COM-порта

# Подготовка данных
        binary_message = ''.join(format(ord(char), '08b') for char in message)


        desired_length = self.DATA_SIZE * 8  # 24 байта = 192 бита
        # Обрезаем до 24 байт и дополняем нулями при необходимости
        if len(binary_message) > desired_length:
            byte_data = byte_data[:desired_length]
        binary_message = binary_message.ljust(desired_length, '0')

        cyclic_code = SystematicCyclicCode(len(binary_message))
        coded_message = cyclic_code.compute_crc(binary_message)


        flag = self.flag[:self.FLAG_SIZE]  
        destination_address = self.destination.decode()
        source_address = source_address
        
        
        

        # Формируем полное сообщение
        final_message_str = f"{flag}{destination_address}{source_address}{coded_message}"
        # Выводим финальное сообщение
        print(f"w-> {final_message_str.encode()}")


        packet = final_message_str.encode()

        parity = self.parity_combobox_send.get() if is_first else self.parity_combobox_receive.get()
        
        response = self.logic.send_packet(connected_port, selected_port, packet, parity)

        self.update_output(cyclic_code, output_text, response, connected_port)
        
        self.bytes_sent = self.FLAG_SIZE + 1 + 1 + self.DATA_SIZE + cyclic_code.fcs_size
        self.update_status_window(self.logic.get_port_speed(selected_port), self.bytes_sent)

    def update_output(self, cyclic_code, output_text, message, connected_port):
        if message is None:
            return

        data_fcs = message[4:]
        print(f"data_fcs : {data_fcs}")

        binary_string = data_fcs.decode('ascii')
        print(f"Бинарная строка: {binary_string}")


        received_message = cyclic_code.introduce_random_error(binary_string)
        print(f"Полученное сообщение с ошибкой: {received_message}")
        # Проверка и исправление сообщения
        corrected_message = cyclic_code.check_and_correct(received_message)
        print(f"Исправленное сообщение: {corrected_message}")

        # Сохранение сообщения с выделенной ошибкой
        highlighted_message = cyclic_code.corrupted_message
        print(f"Сообщение с выделенной ошибкой: {highlighted_message}")

        # Обновление текстового поля с принятым сообщением
        self.current_frame_label.config(text=f"Принято: {message[:4]}{highlighted_message}")


        def binary_to_string(binary_message):
            """Преобразует двоичное сообщение в строку."""
            chars = [chr(int(binary_message[i:i + 8], 2)) for i in range(0, len(binary_message), 8)]
            return ''.join(chars)
        
        # Вывод исправленного сообщения как обычной строки
        corrected_message_text = binary_to_string(corrected_message)
        print(f"\nИсправленное сообщение (текст):     {corrected_message_text}")



        # Обновление окна вывода
        output_text.config(state='normal')
        output_text.insert(tk.END, f"w-> {message}\n")
        output_text.insert(tk.END, f"r-> {message[:4]}{highlighted_message}\n")
 
        output_text.insert(tk.END, f"Data -> {corrected_message_text}\n")
        output_text.config(state='disabled')

    def send_message_first(self, event=None):
        self.send_message(self.input_entry_send, self.left_output_text, self.send_port_combobox, is_first=True)

    def send_message_second(self, event=None):
        self.send_message(self.input_entry_receive, self.right_output_text, self.read_port_combobox, is_first=False)

    def get_connected_port(self, port):
        return self.logic.port_pairs.get(port, "Нет связанного порта")

    def on_closing(self):
        self.master.destroy()