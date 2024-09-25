import tkinter as tk
from tkinter import scrolledtext, ttk, messagebox
from serial_logic import SerialLogic

class SerialInterfaceApp:

    FLAG_SIZE = 2
    flag = "$x" 

    DESTINATION_ADDRESS_SIZE = 1
    destination = bytes([0x30])

    SOURCE_ADDRESS_SIZE = 1
    DATA_SIZE = 24

    FCS_SIZE = 1
    fcs = bytes([0x30])

    def __init__(self, master):
        self.master = master
        self.master.title("Serial Interface")
        self.logic = SerialLogic()

        self.master.minsize(800, 600)  

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
        self.output_frame.pack(pady=10)

        self.left_output_frame = tk.Frame(self.output_frame)
        self.left_output_frame.pack(side=tk.LEFT, padx=10)

        self.right_output_frame = tk.Frame(self.output_frame)
        self.right_output_frame.pack(side=tk.RIGHT, padx=10)

        self.left_output_label = tk.Label(self.left_output_frame, text="Вывод с первого порта:")
        self.left_output_label.pack()

        self.left_output_text = scrolledtext.ScrolledText(self.left_output_frame, width=60, height=10, state='disabled')
        self.left_output_text.pack(padx=5, pady=5)

        self.right_output_label = tk.Label(self.right_output_frame, text="Вывод со второго порта:")
        self.right_output_label.pack()

        self.right_output_text = scrolledtext.ScrolledText(self.right_output_frame, width=60, height=10, state='disabled')
        self.right_output_text.pack(padx=5, pady=5)

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

        byte_data = message.encode()

        # Обрезаем до 24 байт и дополняем нулями при необходимости
        if len(byte_data) > self.DATA_SIZE:
            byte_data = byte_data[:self.DATA_SIZE]
        elif len(byte_data) < self.DATA_SIZE:
            byte_data += b'\x00' * (self.DATA_SIZE - len(byte_data))

        # Формирование полного пакета
        flag = self.flag[:self.FLAG_SIZE]  # Убедитесь, что длина флага 2 байта
        destination_address = self.destination  # Преобразуем адрес назначения в байт
        source_address = source_address.encode()  # Преобразуем адрес источника в байт
        fcs = self.fcs  # Пример контрольной суммы (здесь нужно вычислить реальное значение)

        packet = (
            flag.encode() + 
            destination_address + 
            source_address + 
            byte_data + 
            fcs
        )

        expected_length = self.FLAG_SIZE + self.DESTINATION_ADDRESS_SIZE + self.SOURCE_ADDRESS_SIZE + self.DATA_SIZE + self.FCS_SIZE
        
        if len(packet) != expected_length:
            messagebox.showerror("Ошибка", "Некорректная длина пакета.")
            return

        # Применяем байт-стаффинг ко всему пакету
        stuffed_packet = self.logic.byte_stuffing(packet)
        print(packet)

        parity = self.parity_combobox_send.get() if is_first else self.parity_combobox_receive.get()
        response = self.logic.send_packet(connected_port, selected_port, stuffed_packet, parity)

        self.update_output(output_text, response, connected_port)
        self.bytes_sent += len(stuffed_packet) // 8
        self.update_status_window(self.logic.get_port_speed(selected_port), self.bytes_sent)

    def update_output(self, output_text, message, connected_port):
        if message is None:
            return

        highlighted_message = self.logic.highlight_byte_stuffing(message)
        self.current_frame_label.config(text=f"Принято: {highlighted_message}")

        unstuffed_message = self.logic.de_byte_stuffing(message)
        processed_data = unstuffed_message[4:-1]
        processed_data = processed_data.replace(b'\x00', b'')
        output_data = processed_data.decode('utf-8', errors='ignore')

        output_text.config(state='normal')
        output_text.insert(tk.END, f"-> {unstuffed_message.decode(errors='replace')}\n")
        output_text.insert(tk.END, f"->'f'DS'Data__________________'F \n")
        output_text.insert(tk.END, f"Data -> {output_data}\n\n")

        output_text.config(state='disabled')

    def send_message_first(self, event=None):
        self.send_message(self.input_entry_send, self.left_output_text, self.send_port_combobox, is_first=True)

    def send_message_second(self, event=None):
        self.send_message(self.input_entry_receive, self.right_output_text, self.read_port_combobox, is_first=False)

    def get_connected_port(self, port):
        return self.logic.port_pairs.get(port, "Нет связанного порта")

    def on_closing(self):
        self.master.destroy()