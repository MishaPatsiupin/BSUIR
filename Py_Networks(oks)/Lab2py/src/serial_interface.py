import tkinter as tk
from tkinter import scrolledtext, ttk, messagebox
from serial_logic import SerialLogic

class SerialInterfaceApp:
    # Константы для размеров полей пакета
    FLAG_SIZE = 8
    DESTINATION_ADDRESS_SIZE = 4
    SOURCE_ADDRESS_SIZE = 4
    DATA_SIZE = 23
    FCS_SIZE = 1

    def __init__(self, master):
        self.master = master
        self.master.title("Serial Interface")
        self.logic = SerialLogic()

        # Установка минимальных размеров окна
        self.master.minsize(800, 600)  # Ширина 800, высота 600

        self.setup_ui()
        self.populate_ports()

    def setup_ui(self):
        # Окно для выбора порта отправки
        self.send_frame = tk.Frame(self.master)
        self.send_frame.pack(pady=10)

        self.send_port_label = tk.Label(self.send_frame, text="Пара отправки:")
        self.send_port_label.pack(side=tk.LEFT, padx=5)

        self.send_port_combobox = ttk.Combobox(self.send_frame, width=15)  # Увеличиваем ширину
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

        self.read_port_combobox = ttk.Combobox(self.read_frame, width=15)  # Увеличиваем ширину
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

        self.left_output_text = scrolledtext.ScrolledText(self.left_output_frame, width=60, height=10, state='disabled')  # Увеличиваем ширину
        self.left_output_text.pack(padx=5, pady=5)

        self.right_output_label = tk.Label(self.right_output_frame, text="Вывод со второго порта:")
        self.right_output_label.pack()

        self.right_output_text = scrolledtext.ScrolledText(self.right_output_frame, width=60, height=10, state='disabled')  # Увеличиваем ширину
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

        self.input_label_send = tk.Label(self.input_frame_send, text="Биты (0/1) для первого порта:")
        self.input_label_send.pack(side=tk.LEFT, padx=5)

        # Настройка валидации ввода
        vcmd_send = (self.master.register(self.validate_input), '%S')

        self.input_entry_send = tk.Entry(self.input_frame_send, width=40, validate='key', validatecommand=vcmd_send)
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

        self.input_label_receive = tk.Label(self.input_frame_receive, text="Биты (0/1) для второго порта:")
        self.input_label_receive.pack(side=tk.LEFT, padx=5)

        # Настройка валидации ввода
        vcmd_receive = (self.master.register(self.validate_input), '%S')

        self.input_entry_receive = tk.Entry(self.input_frame_receive, width=40, validate='key', validatecommand=vcmd_receive)
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

    def validate_input(self, char):
        return char in '01'

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
        message = input_entry.get()
        if not message:
            return

        # Проверка на допустимые символы
        if not all(bit in '01' for bit in message):
            messagebox.showerror("Ошибка", "Введите только символы '0' и '1'.")
            return

        selected_port = port_combobox.get()
        connected_port = self.get_connected_port(selected_port)

        if connected_port == "Нет связанного порта":
            messagebox.showerror("Ошибка", "Нет связанного порта для отправки.")
            return

        # Формирование флага
        flag = format(self.DATA_SIZE, '0{}b'.format(self.FLAG_SIZE))  # Двоичное представление DATA_SIZE

        # Формирование адресов
        destination_address = '0000'  # нулевой
        source_address = format(int(selected_port.replace('COM', '')), '0{}b'.format(self.SOURCE_ADDRESS_SIZE))  # Номер COM-порта

        # Убедитесь, что длина данных не превышает фиксированный размер
        if len(message) > self.DATA_SIZE:
            message = message[:self.DATA_SIZE]  # Обрезаем сообщение до максимального размера

        # Бит-стаффинг
        stuffed_data = self.logic.bit_stuffing(message)

        # Заполнение данными до фиксированного размера
        data = stuffed_data.ljust(self.DATA_SIZE, '0')  # Дополняем нулями до DATA_SIZE

        # FCS
        fcs = '0' * self.FCS_SIZE  

        # Формирование полного пакета
        packet = f"{flag}{destination_address}{source_address}{data}{fcs}"

        # Проверка длины пакета
        expected_length = (self.FLAG_SIZE + self.DESTINATION_ADDRESS_SIZE + self.SOURCE_ADDRESS_SIZE + len(data) + self.FCS_SIZE)
        
        if len(packet) != expected_length:
            messagebox.showerror("Ошибка", "Некорректная длина пакета.")
            return

        # Определение паритета
        parity = self.parity_combobox_send.get() if is_first else self.parity_combobox_receive.get()

        # Отправляем пакет
        response = self.logic.send_packet(connected_port, selected_port, packet, parity)
        self.update_output(output_text, response, connected_port)

        # Увеличиваем счетчик переданных байт
        self.bytes_sent += len(packet) // 8
        self.update_status_window(self.logic.get_port_speed(selected_port), self.bytes_sent)

    def update_output(self, output_text, message, connected_port):
        highlighted_message = self.logic.highlight_bit_stuffing(message)
        self.current_frame_label.config(text=f"Принятый кадр: {highlighted_message}")

        unstuffed_message = self.logic.de_bit_stuffing(message)
        output_text.config(state='normal')
        output_text.insert(tk.END, f"-> {unstuffed_message}\n")
        output_text.insert(tk.END, f"->'Flag___'DsA'SoA'Data__________________'F \n")
        output_text.config(state='disabled')

    def send_message_first(self, event=None):
        self.send_message(self.input_entry_send, self.left_output_text, self.send_port_combobox, is_first=True)

    def send_message_second(self, event=None):
        self.send_message(self.input_entry_receive, self.right_output_text, self.read_port_combobox, is_first=False)

    def get_connected_port(self, port):
        return self.logic.port_pairs.get(port, "Нет связанного порта")

    def on_closing(self):
        self.master.destroy()