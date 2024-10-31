import math
import random

class SystematicCyclicCode:
    table_data = {
        1: {'Cтепень полинома': 'х^1 + 1', 'Двоич. код полинома': '11', 'Десятич. код полинома': 3},
        2: {'Cтепень полинома': 'х^2 + х + 1', 'Двоич. код полинома': '111', 'Десятич. код полинома': 7},
        3: {'Cтепень полинома': 'х^3 + х + 1', 'Двоич. код полинома': '1011', 'Десятич. код полинома': 11},
        4: {'Cтепень полинома': 'х^4 + х^3 + х^2 + x + 1', 'Двоич. код полинома': '111111', 'Десятич. код полинома': 31},
        5: {'Cтепень полинома': 'х^5 + х^3 + 1', 'Двоич. код полинома': '101001', 'Десятич. код полинома': 41},
    }

    def __init__(self, data_size):
        self.data_size = data_size
        print(f"Инициализация с размером данных: {self.data_size}")
        self.polynomial = self.calculate_polynomial()
        self.fcs_size = self.crc_bytes_length(self.polynomial)
        print(f"Полином: {self.polynomial}, Размер FCS: {self.fcs_size}")
        self.check_bits = None
        self.corrupted_message = ""  # Переменная для хранения сообщения с выделенной ошибкой

    def get_binary_code_polynomial(self, row_num):
        entry = self.table_data.get(row_num)
        if entry:
            print(f"Используем полином для строки {row_num}: {entry['Двоич. код полинома']}")
            return entry['Двоич. код полинома']
        else:
            print("Используем полином CRC-32 по умолчанию.")
            return '10011000101110111110101100001111'  # Полином CRC-32 в битах

    def calculate_polynomial(self):
        self.X_k = math.ceil(math.log2(self.data_size + math.ceil(math.log2(self.data_size + 1))))
        print(f"Степень полинома (X_k): {self.X_k}")
        return self.get_binary_code_polynomial(self.X_k)

    def crc_bytes_length(self, polynomial_bitstring):
        polynomial_length = len(polynomial_bitstring)
        crc_length_bytes = (polynomial_length - 1 + 7) // 8  # Округление вверх
        print(f"Длина полинома: {polynomial_length}, Длина CRC в байтах: {crc_length_bytes}")
        return crc_length_bytes

    def compute_crc(self, message):
        """Вычисляет CRC для заданного сообщения."""
        padded_message = message + '0' * (len(self.polynomial) - 1)
        
        remainder = self.division(padded_message, self.polynomial)
        self.check_bits = remainder
        
        return message + remainder

    def division(self, dividend, divisor):
        """Выполняет побитное деление."""
        dividend = list(dividend)
        divisor = list(divisor)
        divisor_length = len(divisor)

        for i in range(len(dividend) - len(divisor) + 1):
            if dividend[i] == '1':
                for j in range(divisor_length):
                    dividend[i + j] = str(int(dividend[i + j]) ^ int(divisor[j]))

        remainder = ''.join(dividend[-(divisor_length - 1):])
        return remainder

    def check_and_correct(self, received_message):
        """Проверяет и исправляет сообщение."""
        print(f"Полученное сообщение: {received_message}")
        remainder = self.division(received_message, self.polynomial)

        if '1' not in remainder:
            print("Ошибок не найдено.")
            return received_message[:-len(self.check_bits)]  # Сообщение без ошибок

        # Сохраняем сообщение с выделенной ошибкой
        self.corrupted_message = received_message
        print("Ошибки найдены, начинаем исправление...")

        # Попробуем инвертировать каждый бит
        for i in range(len(received_message)):
            modified_message = list(received_message)
            modified_message[i] = '0' if modified_message[i] == '1' else '1'
            modified_message = ''.join(modified_message)

            new_remainder = self.division(modified_message, self.polynomial)
            if '1' not in new_remainder:
                # Возвращаем исходное сообщение с выделением ошибочного бита
                self.corrupted_message = ''.join(received_message[:i]) + f"__{received_message[i]}__" + ''.join(received_message[i + 1:])
                print(f"Исправленное сообщение: {modified_message}")
                return modified_message[:-len(self.check_bits)]  # Возвращаем исправленное сообщение

        print("Ошибка не может быть исправлена")  # Если не удалось исправить
        return "Ошибка не может быть исправлена"

    def introduce_random_error(self, message):
        """Инвертирует один случайный информационный бит с вероятностью 0.7."""
        message_list = list(message)
        info_bits_length = len(message) - len(self.check_bits)

        if random.random() < 1:  # 70% вероятность изменить бит!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            index_to_corrupt = random.randint(0, info_bits_length - 1)
            original_bit = message_list[index_to_corrupt]

            # Изменяем бит
            message_list[index_to_corrupt] = '0' if original_bit == '1' else '1'
            print(f"\nCorrupt bit at index {index_to_corrupt} changed from {original_bit} to {message_list[index_to_corrupt]}")

        return ''.join(message_list)

    def convert_to_byte_message(self, binary_message, error_position=None):
        """Преобразует двоичное сообщение в байтовое сообщение в формате \x00.
        Если указана ошибка, выделяет ошибочный байт."""
        byte_message = bytearray()  # Используем bytearray для оптимизации

        for i in range(0, len(binary_message), 8):
            byte = binary_message[i:i + 8]
            if len(byte) == 8:  # Убедимся, что байт состоит из 8 бит
                if error_position is not None and (i <= error_position < i + 8):
                    # Если текущий байт содержит ошибку, выделяем его
                    byte_message.append(int(byte, 2))  # Добавляем байт
                else:
                    byte_message.append(int(byte, 2))  # Добавляем байт

        # Преобразуем в строку в формате \x00
        formatted_message = ''.join(f"\\x{byte:02x}" for byte in byte_message)
        print(f"Преобразованное байтовое сообщение: {formatted_message}")
        return formatted_message

    def convert_bytes_to_binary_message(self, byte_string):
        """Преобразует байтовую строку в двоичное сообщение."""
        return ''.join(format(byte, '08b') for byte in byte_string)
'''
# Подготовка данных
data = "hello"
binary_message = ''.join(format(ord(char), '08b') for char in data)  # Преобразуем строку в двоичный формат
print (f"binary_message_134 {binary_message}")
# Дополняем до 24 байт (192 бита)
desired_length = 24 * 8  # 24 байта = 192 бита
binary_message = binary_message.ljust(desired_length, '0')  # Дополняем нулями
print (f"binary_message_139 {binary_message}")
# Инициализация кода
data_size = len(binary_message)  # Размер данных
cyclic_code = SystematicCyclicCode(data_size)

# Кодирование сообщения
coded_message = cyclic_code.compute_crc(binary_message)
print (f"coded_message_145 {coded_message}")
# Введение случайной ошибки
received_message = cyclic_code.introduce_random_error(coded_message)
print (f"received_message_148 {received_message}")
# Проверка и исправление сообщения
corrected_message = cyclic_code.check_and_correct(received_message)
print (f"corrected_message_151 {corrected_message}")

# Вывод информации
print(f"Исходное сообщение:               {binary_message}")
print(f"Кодированное сообщение:           {coded_message}")
print(f"Полученное сообщение (с ошибкой): {received_message}")
print(f"Сообщение с выделенной ошибкой:   {cyclic_code.corrupted_message}")
print(f"Исправленное сообщение:           {corrected_message}")

# Получаем позицию ошибочного бита
error_position = cyclic_code.corrupted_message.find('__')

# Преобразуем полученное сообщение с ошибкой и исправленное сообщение в байтовое представление
byte_message_with_error = cyclic_code.convert_to_byte_message(received_message, error_position)
byte_message_corrected = cyclic_code.convert_to_byte_message(corrected_message)

print(f"\nБайтовое сообщение с ошибкой:")
print(byte_message_with_error)

print(f"\nИсправленное байтовое сообщение:")
print(byte_message_corrected)


print ("\n Обычное с двоичного")
print(corrected_message.encode())

print ("\n Обычное с шеснт")
print(byte_message_corrected.encode())



def binary_to_string(binary_message):
    """Преобразует двоичное сообщение в строку."""
    chars = [chr(int(binary_message[i:i + 8], 2)) for i in range(0, len(binary_message), 8)]
    return ''.join(chars)

# Вывод исправленного сообщения как обычной строки
corrected_message_text = binary_to_string(corrected_message)
print(f"\nИсправленное сообщение (текст):     {corrected_message_text}")
'''