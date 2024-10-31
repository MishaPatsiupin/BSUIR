import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), 'src')))

from serial_interface import SerialInterfaceApp
import tkinter as tk

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialInterfaceApp(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)  # Обрабатываем закрытие окна
    root.mainloop()