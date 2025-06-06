-- Заполнение таблицы поставщиков
-- https://www.b2b.by/produkty-pitaniya-optovaya-torgovlya-PP101/companies/page-1/
INSERT INTO Supplier (companyName, contactInformation, reliabilityRating, productCategory) VALUES
('ЛОГАЛ-БИО ООО', 'logal-bio.by, info@logal-bio.by', 8, 'Грибы'),
('АМИФРУТ ООО', 'www.amifruit.by', 7, 'Азиатские овощи'),
('ЕВРОФОЛИЯ ЧТПУП', '220018, город Минск, Шаранговича 7', 9, 'Упоковочные материалы'),
('ИМПЕРИЯ ЗЛАКОВ ТМ СМОРГОНСКИЙ КОМБИНАТ ХЛЕБОПРОДУКТОВ УПП', ' Беларусь, Гродненская область, Сморгонский район, Сморгонь, Комсомольский переулок, 20', 7, 'Пищевые добавки'),
('ВИГОЛ ООО', 'tastee.by', 8, 'Tastee'),
('ПИЩЕВОЙ КОМБИНАТ БЕЛКООПСОЮЗА ЧУП', '220075, город Минск, пр Партизанский 168', 6, 'Продукты питаня'),
('ДОБРЫЙ ДЕНЬ ООО', '220140, город Минск, Лещинского 8/4-1', 9, 'Продукты питаня'),
('ABC ПЛЮС ФИРМА ООО ПРЕДСТАВИТЕЛЬСТВО', '212011, город Могилев, пер Березовский 1', 7, 'Консервы'),
('КИТАЙСКИЙ ЧАЙ PEKOE.BY МАГАЗИН ЧТУП АДС-ВОСТОК', '210015, город Витебск, Генерала Белобородова 3-65а (ТДЦ Беларусь)', 8, 'Чай'),
('ОРЕХОВАЯ КОМПАНИЯ ООО', '212040, город Могилев, пер 1-й Южный 21 комн 1', 6, 'Орехи'),
('БАКАЛЕЯ МОГИЛЕВ ОАО', '212040, город Могилев, Залуцкого 25', 9, 'Бакалея'),
('ИВАСИ-ТОРГ ЧАСТНОЕ ТОРГОВОЕ УНИТАРНОЕ ПРЕДПРИЯТИЕ', '211388, город Орша, Ленина 230Ц', 7, 'Морепродукты'),
('МИРАНА ООО', '224005, город Брест, Комсомольская 23/1', 8, 'Консервы'),
('ВЕРХНИЙ ЛУГ СООО', '211622, Верхнедвинский район, Партизанская 1, д Янино', 6, 'Молочные продукты'),
('КРАСНЫЙ ПИЩЕВИК ОАО', 'priemnaya@zefir.by', 9, 'Категория П'),
('ЛЕБУР-ПРОДУКТ-ПЛЮС ООО', '223050, Минский район, Базовская 1Б-2, пос Сухорукие', 7, 'Мясо');

-- Заполнение таблицы должностей
INSERT INTO Position (name, salary, workSchedule, responsibilities) VALUES
('Менеджер', 2000.00, '4/3 по 6 часов', 'Управление проектами'),
('Разработчик', 2000.00, '4/3 по 6 часов', 'Разработка ПО'),
('Тестировщик', 1200.00, '4/3 по 6 часов', 'Тестирование ПО'),
('Аналитик', 2500.00, '4/3 по 6 часов', 'Анализ данных'),
('Шеф-повар', 2500.00, '2/2 по 12 часов', 'Руководство кухней'),
('Су-шеф', 2000.00, '2/2 по 12 часов', 'Помощь шеф-повару'),
('Повар', 1500.00, '2/2 по 12 часов', 'Приготовление блюд'),
('Кондитер', 1400.00, '2/2 по 12 часов', 'Приготовление десертов'),
('Пекарь', 1300.00, '2/2 по 12 часов', 'Выпечка хлебобулочных изделий'),
('Бариста', 1200.00, '2/2 по 12 часов', 'Приготовление кофе'),
('Официант', 1000.00, '2/2 по 12 часов', 'Обслуживание клиентов'),
('Хостес', 1100.00, '2/2 по 12 часов', 'Встреча гостей'),
('Администратор', 1500.00, '2/2 по 12 часов', 'Управление персоналом'),
('Кассир', 900.00, '2/2 по 12 часов', 'Работа на кассе'),
('Уборщик', 800.00, '2/2 по 12 часов', 'Уборка помещений'),
('Мойщик посуды', 700.00, '2/2 по 12 часов', 'Мытье посуды'),
('Закупщик', 1500.00, '2/2 по 12 часов', 'Закупка продуктов'),
('Маркетолог', 2500.00, '4/3 по 6 часов', 'Маркетинговые исследования'),
('Бухгалтер', 1500.00, '2/2 по 12 часов', 'Ведение бухгалтерии'),
('Юрист', 2000.00, '2/2 по 12 часов', 'Юридическое сопровождение'),
('HR-менеджер', 1500.00, '2/2 по 12 часов', 'Управление персоналом'),
('Техник', 1400.00, '2/2 по 12 часов', 'Обслуживание оборудования'),
('Водитель', 1200.00, '2/2 по 12 часов', 'Доставка продуктов'),
('Охранник', 1000.00, '2/2 по 12 часов', 'Охрана территории'),
('Сомелье', 1500.00, '2/2 по 12 часов', 'Подбор вин'),
('Бармен', 1200.00, '2/2 по 12 часов', 'Приготовление напитков'),
('Заведующий складом', 1400.00, '2/2 по 12 часов', 'Управление складом'),
('Курьер', 1000.00, '2/2 по 12 часов', 'Доставка заказов'),
('Дизайнер', 2500.00, '4/3 по 6 часов', 'Дизайн интерьера'),
('PR-менеджер', 2500.00, '4/3 по 6 часов', 'Связи с общественностью');

-- Заполнение таблицы сотрудников
INSERT INTO Employee (fullName, phoneNumber, email, hireDate, contractEndDate) VALUES
('Бакунович Никита Андреевич', '+375291234501', 'bakunovich@lido.by', '2025-03-01', '2026-03-01'),
('Велич Никита Олегович', '+375291234502', 'velich@lido.by', '2025-03-02', '2026-03-02'),
('Георгиев Никита Димитров', '+375291234503', 'georgiev@lido.by', '2025-03-03', '2027-03-03'),
('Говор Павел Сергеевич', '+375291234504', 'govor@lido.by', '2025-03-04', '2025-09-04'),
('Горчаков Никита Сергеевич', '+375291234505', 'gorchakov@lido.by', '2025-03-05', '2026-03-05'),
('Гусаков Святослав', '+375291234506', 'gusakov@lido.by', '2025-03-01', '2027-03-01'),
('Демидович Руслан Сергеевич', '+375291234507', 'demidovich@lido.by', '2025-03-02', '2025-09-02'),
('Дылевский Егор Олегович', '+375291234508', 'dylevski@lido.by', '2025-03-03', '2026-03-03'),
('Жуковская Вероника Кирилловна', '+375291234509', 'zhukovskaya@lido.by', '2025-03-04', '2027-03-04'),
('Золотницкий Алексей Андреевич', '+375291234510', 'zolotnitski@lido.by', '2025-03-05', '2025-09-05'),
('Каражан Ксения Александровна', '+375291234511', 'karajan@lido.by', '2025-03-01', '2026-03-01'),
('Коледа Анна Валерьевна', '+375291234512', 'koleda@lido.by', '2025-03-02', '2027-03-02'),
('Корзун Ксения Игоревна', '+375291234513', 'korzun@lido.by', '2025-03-03', '2025-09-03'),
('Кузьмин Дмитрий Сергеевич', '+375291234514', 'kuzmin@lido.by', '2025-03-04', '2026-03-04'),
('Курдеко Никита Андреевич', '+375291234515', 'kurdeko@lido.by', '2025-03-05', '2027-03-05'),
('Курчицкий Глеб Анатольевич', '+375291234516', 'kurchitski@lido.by', '2025-03-01', '2025-09-01'),
('Лемешонок Дмитрий Андреевич', '+375291234517', 'lemeshonok@lido.by', '2025-03-02', '2026-03-02'),
('Мащенко Артур Витальевич', '+375291234518', 'mashchenko@lido.by', '2025-03-03', '2027-03-03'),
('Мелюх Евгений Сергеевич', '+375291234519', 'melyukh@lido.by', '2025-03-04', '2025-09-04'),
('Можейко Дмитрий Олегович', '+375291234520', 'mozheiko@lido.by', '2025-03-05', '2026-03-05'),
('Мынзул Александр Эдуардович', '+375291234521', 'minzul@lido.by', '2025-03-01', '2027-03-01'),
('Николаев Иван Ярославович', '+375291234522', 'nikolaev@lido.by', '2025-03-02', '2025-09-02'),
('Осинин Иван Николаевич', '+375291234523', 'osinin@lido.by', '2025-03-03', '2026-03-03'),
('Патюпин Михаил Сергеевич', '+375291234524', 'patyupin@lido.by', '2025-03-04', '2027-03-04'),
('Приловский Виктор Витальевич', '+375291234525', 'prilovski@lido.by', '2025-03-05', '2025-09-05'),
('Рыбак Феона Игоревна', '+375291234526', 'rybak@lido.by', '2025-03-01', '2026-03-01'),
('Савинич Тимофей Максимович', '+375291234527', 'savinich@lido.by', '2025-03-02', '2027-03-02'),
('Хорошун Никита Андреевич', '+375291234528', 'khoroshun@lido.by', '2025-03-03', '2025-09-03'),
('Черняк Станислав Сергеевич', '+375291234529', 'chernyak@lido.by', '2025-03-04', '2026-03-04'),
('Шагун Денис Валерьевич', '+375291234530', 'shagun@lido.by', '2025-03-05', '2027-03-05'),
('Шах Евгений Дмитриевич', '+375291234531', 'shakh@lido.by', '2025-03-01', '2025-09-01');

-- Заполнение таблицы блюд
INSERT INTO Dish (name, weight, nutritionalValue, cost, category) VALUES
('Горчица', 60.00, 'Белки –7,1 г; Жиры – 8,7 г; Углеводы – 12,0 г; Энергетическая ценность – 157,6 Ккал/659 кДж', 2.00, 'Соусы'),
('Блины с картофелем и грибами', 135.00, 'Белки –6,9 г; Жиры – 9,7 г; Углеводы – 26,3 г; Энергетическая ценность – 221 Ккал/925 кДж', 3.00, 'Блины'),
('Суп сырный с грибами', 280.00, 'Белки – 1,27г; Жиры – 2,3; Углеводы – 5,0 г; Энергетическая ценность – 49 Ккал/205 кДж', 2.40, 'Супы'),
('Суп куриный', 280.00, 'Белки – 3,6 г; Жиры – 2,2 г; Углеводы – 3,8 г; Энергетическая ценность – 50 Ккал/209 кДж', 2.80, 'Супы'),
('Солянка мясная', 280.00, 'Белки – 1,9 г; Жиры – 6,9 г; Углеводы – 2,7 г; Энергетическая ценность – 84 Ккал/351 кДж', 4.50, 'Супы'),
('Колбаса "Куриная с майораном" жареная', 100.00, 'Белки – 20,5 г; Жиры – 33,3 г; Углеводы – 0,9 г; Энергетическая ценность – 387 Ккал/1618 кДж', 5.30, 'Горячие блюда'),
('Скумбрия гриль', 100.00, 'Белки – 21,3 г; Жиры – 16,1 г; Углеводы – 0,9 г; Энергетическая ценность – 235 Ккал/983 кДж', 6.00, 'Гриль'),
('Шашлык из свинины "Фирменный"', 150.00, 'Белки – 19,4 г; Жиры – 45,5 г; Углеводы – 0,9 г; Энергетическая ценность – 496 Ккал/2075 кДж', 13.50, 'Гриль'),
('Шашлык из птицы "Оригинальный"', 160.00, 'Белки – 29,1 г; Жиры – 6,6 г; Углеводы – 2,1 г; Энергетическая ценность – 183 Ккал/766 кДж', 9.50, 'Гриль'),
('Окорочка "Ароматные"', 100.00, 'Белки – 26,0 г; Жиры – 21,0 г; Углеводы – 4,1 г; Энергетическая ценность – 305 Ккал/1275 кДж', 3.80, 'Горячие блюда'),
('Хлеб "Мамма Миа"', 60.00, ' Белки –11,0 г; Жиры – 4,0 г; Углеводы – 46 г; Энергетическая ценность – 260 Ккал/1120 кДж', 1.00, 'Выпечка'),
('Чиабатта', 100.00, 'Белки – 8 г; Жиры – 2 г; Углеводы – 52 г; Энергетическая ценность – 260 Ккал/1100 кДж', 1.00, 'Выпечка'),
('Хлеб', 35.00, 'Белки – 6,0 г; Жиры – 0,5 г; Углеводы – 48,0 г; Энергетическая ценность – 220 Ккал/950 кДж', 0.13, 'Выпечка'),
('Напиток из шиповника и боярышника', 250.00, 'Белки – 0,3 г; Жиры – 0 г; Углеводы – 8,6 г; Энергетическая ценность – 35 Ккал/146 кДж', 1.90, 'Напитки'),
('Топпинг клубничный', 60.00, 'Белки – 0,0 г; Жиры – 0,0 г; Углеводы – 69,0 г; Энергетическая ценность – 264 Ккал/1105 кДж', 2.50, 'Соусы'),
('Аджика "Домашняя"', 60.00, 'Белки – 1 г; Жиры – 4,6 г; Углеводы – 9,1 г', 0.90, 'Соусы'),
('Десерт "Вишневый пломбир"', 155.00, 'Белки –2,7 г; Жиры – 12,1 г; Углеводы – 33,8 г; Энергетическая ценность – 261 Ккал/1092 кДж', 6.0, 'Десерты'),
('Фруктовый коктейль', 170.00, 'Белки –0,3 г; Жиры – 0,1 г; Углеводы – 11,7 г; Энергетическая ценность – 48 Ккал/202 кДж', 7.0, 'Напитки'),
('Сметана', 55.00, 'Белки –2,6 г; Жиры – 18,0 г; Углеводы – 2,9 г; Энергетическая ценность – 184 Ккал/770,4 кДж', 0.9, 'Молочные продукты'),
('Кефир', 240.00, 'Белки –3,1 г; Жиры – 2,6 г; Углеводы – 4,0 г; Энергетическая ценность – 50 Ккал/220 кДж', 1.2, 'Молочные продукты'),
('Йогурт питьевой', 250.00, 'Белки –2,9 г; Жиры – 1,0 г; Углеводы – 11,5 г; Энергетическая ценность – 66,6 Ккал/281,8 кДж', 1.2, 'Молочные продукты'),
('Блины с начинкой из вишни', 135.00, 'Белки – 5,6 г; Жиры – 6,3 г; Углеводы – 37,5 г; Энергетическая ценность – 226 Ккал/945 кДж', 3.5, 'Блины'),
('Блины с творогом', 135.00, 'Белки –10 г; Жиры – 7,4 г; Углеводы – 28,1 г; Энергетическая ценность – 216 Ккал/904 кДж', 2.7, 'Блины'),
('Блины с мясом цыпленка', 135.00, 'Белки –12,3 г; Жиры – 9 г; Углеводы – 23,5 г; Энергетическая ценность – 222 Ккал/929 кДж', 3.5, 'Блины'),
('Блины с сыром и ветчиной', 135.00, 'Белки –14,2 г; Жиры – 14,1 г; Углеводы – 22,2 г; Энергетическая ценность – 271 Ккал/1134 кДж', 4.0, 'Блины'),
('Блины с яблоками и клюквой', 135.00, 'Белки –5,3 г; Жиры – 6,3 г; Углеводы – 32,7 г; Энергетическая ценность – 207 Ккал/866 кДж', 2.7, 'Блины'),
('Шампиньоны "Ароматные"', 100.00, 'Белки – 6,4 г; Жиры – 10,3 г; Углеводы – 0,1 г; Энергетическая ценность – 123 Ккал/515 кДж', 4.3, 'Овощи'),
('Блины, 2 шт', 180.00, 'Белки – 7,9 г; Жиры – 4,5 г; Углеводы – 33,2 г; Энергетическая ценность – 201 Ккал/841 кДж', 2.2, 'Блины'),
('Стручковая фасоль с морковью и кукурузой', 150.00, 'Белки – 1,8 г; Жиры – 10,2 г; Углеводы – 7,1 г; Энергетическая ценность – 132 Ккал/552 кДж', 5.25, 'Овощи'),
('Микс овощной', 150.00, 'Белки – 1,0 г; Жиры – 0 г; Углеводы – 3,3 г; Энергетическая ценность – 19 Ккал/79 кДж', 6.45, 'Овощи'),
('Картофель отварной обжаренный', 150.00, 'Белки – 2,3 г; Жиры – 14,1 г; Углеводы – 18,1 г; Энергетическая ценность – 219 Ккал/916 кДж', 2.25, 'Картофель'),
('Картофель в кожуре обжаренный', 150.00, 'Белки – 2,3 г; Жиры – 14,1 г; Углеводы – 22,6 г; Энергетическая ценность – 222 Ккал/929 кДж', 1.8, 'Картофель'),
('Рис с куркумой', 150.00, 'Белки – 2,7 г; Жиры – 3,9 г; Углеводы – 26,5 г; Энергетическая ценность – 154 Ккал/644 кДж', 1.8, 'Гарниры'),
('Гречка отварная', 150.00, 'Белки – 5,7 г; Жиры – 3,4 г; Углеводы – 26,1 г; Энергетическая ценность – 160 Ккал/669 кДж', 1.2, 'Гарниры'),
('Драники по-домашнему', 200.00, 'Белки – 4,8 г; Жиры – 15,7 г; Углеводы – 34,9 г; Энергетическая ценность – 293 Ккал/1225 кДж', 4.5, 'Горячие блюда'),
('Картофельное пюре', 200.00, 'Белки – 2,1 г; Жиры – 2,6 г; Углеводы – 16,7 г; Энергетическая ценность – 94 Ккал/393 кДж', 2.0, 'Гарниры'),
('Картофель жареный соломкой', 150.00, 'Белки – 3,9 г; Жиры – 10,9 г; Углеводы – 30,5 г; Энергетическая ценность – 250 Ккал/1046 кДж', 3.0, 'Картофель'),
('Картофель отварной с укропом', 150.00, 'Белки – 1,9 г; Жиры – 0,4 г; Углеводы – 18,6 г; Энергетическая ценность – 80 Ккал/334 кДж', 1.25, 'Картофель'),
('Котлета из свинины', 95.00, 'Белки – 12,2 г; Жиры – 39,1 г; Углеводы – 9,8 г; Энергетическая ценность – 446 Ккал/1864 кДж', 4.2, 'Мясо'),
('Говядина с грибами под сыром', 140.00, 'Белки – 16,6 г; Жиры – 26,8 г; Углеводы – 5,7 г; Энергетическая ценность – 331 Ккал/1384 кДж', 9.3, 'Мясо');
/*
('Котлета говяжья', 100.00, 'Белки – 14,7 г; Жиры – 31,7 г; Углеводы – 11,4 г; Энергетическая ценность – 395 Ккал/1651 кДж', 5.8, 'Мясо'),
('Филе хека с морковью, блюдо весовое', 100.00, 'Белки – 11,6 г; Жиры – 13,8 г; Углеводы – 5,7 г; Энергетическая ценность – 195 Ккал/816 кДж', 5.8, 'Рыба'),
('Филе хека с помидором и шпинатом, блюдо весовое', 100.00, 'Белки – 15,8 г; Жиры – 21,6 г; Углеводы – 3,7 г; Энергетическая ценность – 274 Ккал/1146 кДж', 6.5, 'Рыба'),
('Свинина по-охотничьи', 140.00, 'Белки – 14,9 г; Жиры – 41,7 г; Углеводы – 5,9 г; Энергетическая ценность – 463 Ккал/1937 кДж', 8.5, 'Мясо'),
('Свинина, запеченная с ветчиной и огурцом', 140.00, 'Белки – 14,0 г; Жиры – 37,9 г; Углеводы – 4,6 г; Энергетическая ценность – 425 Ккал/1778 кДж', 8.5, 'Мясо'),
('Котлета из птицы', 100.00, 'Белки – 17,3 г; Жиры – 20,1 г; Углеводы – 7,8 г; Энергетическая ценность – 285 Ккал/1192 кДж', 4.2, 'Птица'),
('Плов из птицы', 125.00, 'Белки – 11,3 г; Жиры – 17,6 г; Углеводы – 18,3 г; Энергетическая ценность – 279 Ккал/1167 кДж', 9.0, 'Птица'),
('Котлета рыбная из хека', 90.00, 'Белки – 12,4 г; Жиры – 23,2 г; Углеводы – 8,4 г; Энергетическая ценность – 293 Ккал/1226 кДж', 4.8, 'Рыба'),
('Куриная отбивная', 110.00, 'Белки – 26,1 г; Жиры – 17,3 г; Углеводы – 4,3 г; Энергетическая ценность – 277 Ккал/1159 кДж', 5.5, 'Птица'),
('Филе куриное с сыром и помидором', 160.00, 'Белки – 22,4 г; Жиры – 23,1 г; Углеводы – 5,2 г; Энергетическая ценность – 321 Ккал/1343 кДж', 9.0, 'Птица'),
('Филе куриное со шпинатом', 160.00, 'Белки – 22,6 г; Жиры – 23,1 г; Углеводы – 5,2 г; Энергетическая ценность – 321 Ккал/1343 кДж', 9.00, 'Птица'),
('Котлета куриная паровая с брокколи', 95.00, 'Белки – 16,7 г; Жиры – 8,6 г; Углеводы – 5,8 г; Энергетическая ценность – 165 Ккал/690 кДж', 4.20, 'Птица'),
('Микс овощной "Огурец-помидор"', 100.00, 'Белки – 1,0 г; Жиры – 0 г; Углеводы – 3,3 г; Энергетическая ценность – 19 Ккал/79 кДж', 2.50, 'Овощи'),
('Огурцы консервированные', 60.00, 'Белки – 0,1 г; Жиры – 0,7 г; Углеводы – 2,4 г; Энергетическая ценность – 64 Ккал/267 кДж', 2.00, 'Овощи'),
('Салат "Легкий" с морской капустой', 120.00, 'Белки – 2 г; Жиры – 10,4 г; Углеводы – 2 г; Энергетическая ценность – 100 Ккал/418 кДж', 3.00, 'Салаты'),
('Салат "Оливье"', 130.00, 'Белки – 6,5 г; Жиры – 19,3 г; Углеводы – 6,8 г; Энергетическая ценность – 231 Ккал/967 кДж', 4.00, 'Салаты'),
('Корейская морковь', 100.00, 'Белки – 1,2 г; Жиры – 15,0 г; Углеводы – 11,6 г; Энергетическая ценность – 190 Ккал/795 кДж', 1.80, 'Салаты'),
('Салат из курицы с красной фасолью', 130.00, 'Белки – 9,2 г; Жиры – 13,5 г; Углеводы – 5,2 г; Энергетическая ценность – 182 Ккал/761 кДж', 4.50, 'Салаты'),
('Салат из свежих огурцов', 100.00, 'Белки – 1,1 г; Жиры – 4,1 г; Углеводы – 2,2 г; Энергетическая ценность – 50 Ккал/209 кДж', 1.80, 'Салаты'),
('Салат из помидоров и огурцов со сметаной', 130.00, 'Белки – 1,3 г; Жиры – 5 г; Углеводы – 3,0 г; Энергетическая ценность – 64 Ккал/268 кДж', 3.00, 'Салаты'),
('Салат по-гречески с мягким сыром', 140.00, 'Белки – 4,5 г; Жиры – 11,2 г; Углеводы – 2,6 г; Энергетическая ценность – 132 Ккал/552 кДж', 4.50, 'Салаты'),
('Сельдь под шубой', 165.00, 'Белки – 6,9 г; Жиры – 16,1 г; Углеводы – 9,0 г; Энергетическая ценность – 232 Ккал/969 кДж', 4.00, 'Салаты');
*/


-- Заполнение таблицы посетителей
INSERT INTO Visitor (name, birthDate, phoneNumber, preferences) VALUES
('Никульшин Борис Викторович', '01-01', '+375172932379', 'Предпочитает вегетарианские блюда, любит столик у окна'),
('Куприянова Диана Вячеславовна', '15-02', '+375172938617', 'Предпочитает блюда с морепродуктами, любит столик на террасе'),
('Перцев Дмитрий Юрьевич', '10-03', '+375172938039', 'Предпочитает блюда с низким содержанием жира, любит столик в углу'),
('Татур Михаил Михайлович', '25-04', '+375172938564', 'Предпочитает мясные блюда, любит столик у камина'),
('Старовойтов Валерий Васильевич', '30-05', NULL, 'Предпочитает морепродукты, любит столик на террасе'),
('Воронов Александр', '05-06', NULL, 'Предпочитает блюда с высоким содержанием белка, любит столик у окна'),
('Кобяк Игорь', '20-07', '+375172938569', 'Предпочитает блюда с высоким содержанием белка, любит столик в углу'),
('Луцик Юрий', '15-08', '+375172938697', 'Предпочитает блюда без сахара, любит столик у камина'),
('Одинец Дмитрий', '10-09', '+375172932389', 'Предпочитает блюда без сахара, любит столик у окна'),
('Селезнёв Игорь', '25-10', '+375172938569', 'Предпочитает блюда с низким содержанием углеводов, любит столик на террасе'),
('Фролов Игорь', '30-11', NULL, 'Предпочитает блюда с низким содержанием углеводов, любит столик в углу'),
('Байрак Сергей', '05-12', '+375172932389', 'Предпочитает веганские блюда, любит столик у камина'),
('Глецевич Иван', '10-01', '+375172938587', 'Предпочитает веганские блюда, любит столик у окна'),
('Ковальчук Анна', '20-02', '+375172938697', 'Предпочитает блюда с высоким содержанием клетчатки, любит столик на террасе'),
('Лукьянова Ирина', '15-03', '+375172938697', 'Предпочитает блюда с высоким содержанием клетчатки, любит столик в углу'),
('Поденок Леонид', '10-04', '+375172938039', 'Предпочитает блюда с низким содержанием жира, любит столик у камина'),
('Смирнов Юрий', '05-05', '+375172938597', 'Предпочитает блюда с низким содержанием жира, любит столик у окна'),
('Андриевский Егор', '20-06', '+375172938039', 'Предпочитает блюда с высоким содержанием белка, любит столик на террасе'),
('Богдан Евгений', '15-07', '+375172938039', 'Предпочитает блюда без сахара, любит столик в углу'),
('Внук Ольга', '10-08', '+375172938840', 'Предпочитает блюда с низким содержанием углеводов, любит столик у камина'),
('Жалейко Дмитрий', '05-09', '+375172932222', 'Предпочитает блюда с низким содержанием углеводов, любит столик у окна'),
('Желтко Юлия', '20-10', '+375172938039', 'Предпочитает веганские блюда, любит столик на террасе'),
('Ковшер Елизавета', '15-11', '+375172938697', 'Предпочитает веганские блюда, любит столик в углу'),
('Крюков Алексей', '10-12', '+375172938697', 'Предпочитает блюда с высоким содержанием клетчатки, любит столик у камина'),
('Марцинкевич Владислав', '25-01', '+375172938691', 'Предпочитает блюда с высоким содержанием клетчатки, любит столик у окна'),
('Протько Мария', '05-02', NULL, 'Предпочитает блюда с низким содержанием жира, любит столик на террасе'),
('Савчиц Андрей', '10-03', NULL, 'Предпочитает блюда с низким содержанием жира, любит столик в углу'),
('Селезнёв Александр', '15-04', '+375172938569', 'Предпочитает блюда с высоким содержанием белка, любит столик у камина'),
('Скиба Ирина', '20-05', '+375172938698', 'Предпочитает блюда без сахара, любит столик у окна'),
('Стракович Андрей', '25-06', '+375172938589', 'Предпочитает блюда с низким содержанием углеводов, любит столик на террасе'),
('Тарасюк Игорь', '30-07', '+375172932222', 'Предпочитает блюда с низким содержанием углеводов, любит столик в углу'),
('Туровец Николай', '05-08', '+375172938589', 'Предпочитает веганские блюда, любит столик у камина'),
('Хатеневич Артемий', '10-09', '+375172938566', 'Предпочитает веганские блюда, любит столик у окна'),
('Шеменков Владислав', '15-10', '+375172932389', 'Предпочитает блюда с высоким содержанием клетчатки, любит столик на террасе'),
('Марченков Дмитрий', '20-11', NULL, 'Предпочитает блюда с высоким содержанием клетчатки, любит столик в углу');