import tkinter as tk
from tkinter import scrolledtext, ttk, messagebox
from serial_logic import SerialLogic

class SerialInterfaceApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Serial Interface")
        self.logic = SerialLogic()

        self.setup_ui()
        self.populate_ports()

    def setup_ui(self):
        # Окно для выбора порта отправки
        self.send_frame = tk.Frame(self.master)
        self.send_frame.pack(pady=10)

        self.send_port_label = tk.Label(self.send_frame, text="Порт отправки:")
        self.send_port_label.pack(side=tk.LEFT, padx=5)

        self.send_port_combobox = ttk.Combobox(self.send_frame, width=10)
        self.send_port_combobox.pack(side=tk.LEFT, padx=5)

        self.send_connected_port_label = tk.Label(self.send_frame, text="Связанный порт:")
        self.send_connected_port_label.pack(side=tk.LEFT, padx=5)

        self.send_connected_port = tk.Label(self.send_frame, text="")
        self.send_connected_port.pack(side=tk.LEFT, padx=5)

        self.send_port_combobox.bind("<<ComboboxSelected>>", self.update_connected_send_port)

        # Окно для выбора порта чтения
        self.read_frame = tk.Frame(self.master)
        self.read_frame.pack(pady=10)

        self.read_port_label = tk.Label(self.read_frame, text="Порт для чтения:")
        self.read_port_label.pack(side=tk.LEFT, padx=5)

        self.read_port_combobox = ttk.Combobox(self.read_frame, width=10)
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

        self.left_output_text = scrolledtext.ScrolledText(self.left_output_frame, width=40, height=10, state='disabled')
        self.left_output_text.pack(padx=5, pady=5)

        self.right_output_label = tk.Label(self.right_output_frame, text="Вывод со второго порта:")
        self.right_output_label.pack()

        self.right_output_text = scrolledtext.ScrolledText(self.right_output_frame, width=40, height=10, state='disabled')
        self.right_output_text.pack(padx=5, pady=5)

        # Параметры паритета для первого порта (перемещено под окно вывода)
        self.parity_frame_send = tk.Frame(self.master)
        self.parity_frame_send.pack(pady=10)

        self.parity_label_send = tk.Label(self.parity_frame_send, text="Паритет первого порта:")
        self.parity_label_send.pack(side=tk.LEFT, padx=5)

        self.parity_combobox_send = ttk.Combobox(self.parity_frame_send, values=["None", "Even", "Odd"], state='readonly', width=10)
        self.parity_combobox_send.current(0)  # По умолчанию None
        self.parity_combobox_send.pack(side=tk.LEFT, padx=5)

        # Окно ввода сообщения для первого порта (перемещено под окно вывода)
        self.input_frame_send = tk.Frame(self.master)
        self.input_frame_send.pack(pady=10)

        self.input_label_send = tk.Label(self.input_frame_send, text="Сообщение для первого порта:")
        self.input_label_send.pack(side=tk.LEFT, padx=5)

        self.input_entry_send = tk.Entry(self.input_frame_send, width=40)
        self.input_entry_send.pack(side=tk.LEFT, padx=5)
        self.input_entry_send.bind("<Return>", self.send_message_first)

        # Параметры паритета для второго порта (перемещено под окно вывода)
        self.parity_frame_receive = tk.Frame(self.master)
        self.parity_frame_receive.pack(pady=10)

        self.parity_label_receive = tk.Label(self.parity_frame_receive, text="Паритет второго порта:")
        self.parity_label_receive.pack(side=tk.LEFT, padx=5)

        self.parity_combobox_receive = ttk.Combobox(self.parity_frame_receive, values=["None", "Even", "Odd"], state='readonly', width=10)
        self.parity_combobox_receive.current(0)  # По умолчанию None
        self.parity_combobox_receive.pack(side=tk.LEFT, padx=5)

        # Окно ввода сообщения для второго порта (перемещено под окно вывода)
        self.input_frame_receive = tk.Frame(self.master)
        self.input_frame_receive.pack(pady=10)

        self.input_label_receive = tk.Label(self.input_frame_receive, text="Сообщение для второго порта:")
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

        self.bytes_sent = 0

    def start_reading_from_port(self, port, parity, output_text_widget):
        def update_output(data):
            output_text_widget.config(state='normal')
            output_text_widget.insert(tk.END, data)
            output_text_widget.config(state='disabled')

        self.logic.start_read_thread(port, parity, update_output)


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
            messagebox.error("Ошибка", str(e))

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

    def send_message(self, input_entry, output_text, port_combobox, parity_combobox, is_first=True):
        message = input_entry.get()
        if not message:
            return

        selected_port = port_combobox.get()
        connected_port = self.get_connected_port(selected_port)

        if connected_port == "Нет связанного порта":
            messagebox.showerror("Ошибка", "Нет связанного порта для отправки.")
            return

        parity = parity_combobox.get()
        port_speed = self.logic.get_port_speed(selected_port)
        self.bytes_sent = 0

        def send_next_char(index):
            if index < len(message):
                char = message[index]
                response = self.logic.send_and_receive_message(connected_port, selected_port, char, parity)
                self.update_output(output_text, char, connected_port, response)
                self.bytes_sent += 1
                self.update_status_window(port_speed, self.bytes_sent)
                self.master.after(100, send_next_char, index + 1)

        send_next_char(0)  

    def update_output(self, output_text, message, connected_port, response):
        output_text.config(state='normal')
        if response:
            output_text.insert(tk.END, f"Получено : {response}\n")
        else:
            output_text.insert(tk.END, f"Не удалось получить ответ от {connected_port}.\n")
        output_text.config(state='disabled')
        
    def send_message_first(self, event=None):
        self.send_message(self.input_entry_send, self.left_output_text, self.send_port_combobox, self.parity_combobox_send)

    def send_message_second(self, event=None):
        self.send_message(self.input_entry_receive, self.right_output_text, self.read_port_combobox, self.parity_combobox_receive, is_first=False)

    def get_connected_port(self, port):
        return self.logic.port_pairs.get(port, "Нет связанного порта")

    def on_closing(self):
        self.logic.stop_read_thread()
        self.master.destroy()
