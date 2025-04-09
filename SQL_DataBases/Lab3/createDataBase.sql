-- Таблица поставщиков
CREATE TABLE Supplier (
    supplierId SERIAL PRIMARY KEY,
    companyName VARCHAR(100),
    contactInformation VARCHAR(255),
    reliabilityRating INT,
    productCategory VARCHAR(100)
);

-- Таблица должностей
CREATE TABLE Position (
    positionId SERIAL PRIMARY KEY,
    name VARCHAR(100),
    salary DECIMAL(10, 2),
    workSchedule VARCHAR(100),
    responsibilities TEXT
);

-- Таблица сотрудников
CREATE TABLE Employee (
    employeeId SERIAL PRIMARY KEY,
    fullName VARCHAR(200),
    phoneNumber VARCHAR(15),
    email VARCHAR(100),
    hireDate DATE,
    contractEndDate DATE
);

-- Таблица блюд
CREATE TABLE Dish (
    dishId SERIAL PRIMARY KEY,
    name VARCHAR(100),
    weight DECIMAL(10, 2),
    nutritionalValue VARCHAR(100),
    cost NUMERIC(10, 2),
    category VARCHAR(100)
);

-- Таблица ингредиентов
CREATE TABLE Ingredient (
    ingredientId SERIAL PRIMARY KEY,
    name VARCHAR(100),
    unitOfMeasurement VARCHAR(50),
    inStock DECIMAL(10, 2),
    expirationDate DATE,
    supplierId INT
);

-- Таблица посетителей
CREATE TABLE Visitor (
    visitorId SERIAL PRIMARY KEY,
    name VARCHAR(200),
    birthDate DATE,
    phoneNumber VARCHAR(15),
    preferences TEXT
);

-- Таблица заказов
CREATE TABLE "Order" (
    orderId SERIAL PRIMARY KEY,
    status VARCHAR(50),
    totalCost DECIMAL(10, 2),
    numberOfGuests INT,
    orderDateTime TIMESTAMP,
    visitorId INT,
    employeeId INT
);

-- Таблица связей сотрудников и должностей
CREATE TABLE EmployeePosition (
    employeeId INT,
    positionId INT,
    PRIMARY KEY (employeeId, positionId)
);

-- Таблица связей блюд и ингредиентов
CREATE TABLE DishIngredient (
    dishId INT,
    ingredientId INT,
    quantity DECIMAL(10, 2),
    PRIMARY KEY (dishId, ingredientId)
);

-- Таблица связей заказов и блюд
CREATE TABLE OrderDish (
    orderId INT,
    dishId INT,
    PRIMARY KEY (orderId, dishId)
);

-- Добавление ограничений с помощью ALTER TABLE
ALTER TABLE Supplier
    ALTER COLUMN companyName SET NOT NULL,
    ADD CONSTRAINT companyName_check CHECK (companyName ~ '^[a-zA-Zа-яА-Я0-9 ,.-]+$'),
    ADD CONSTRAINT reliabilityRating_check CHECK (reliabilityRating BETWEEN 1 AND 10);

ALTER TABLE Position
    ALTER COLUMN name SET NOT NULL,
    ADD CONSTRAINT name_check CHECK (name ~ '^[a-zA-Zа-яА-ЯёЁ0-9 ,."-]+$'),
    ALTER COLUMN salary SET NOT NULL,
    ADD CONSTRAINT salary_check CHECK (salary >= 0);

ALTER TABLE Employee
    ALTER COLUMN fullName SET NOT NULL,
    ADD CONSTRAINT fullName_check CHECK (fullName ~ '^[a-zA-Zа-яА-Я0-9 ,.-]+$'),
    ADD CONSTRAINT phoneNumber_check CHECK (phoneNumber ~ '^\+375[0-9]{9}$'),
    ADD CONSTRAINT email_check CHECK (email ~* '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$'),
    ADD CONSTRAINT contractEndDate_check CHECK (contractEndDate >= hireDate);

ALTER TABLE Dish
    ALTER COLUMN name SET NOT NULL,
    ADD CONSTRAINT name_check CHECK (name ~ '^[a-zA-Zа-яА-ЯёЁ0-9 ,."-]+$'),
    ALTER COLUMN weight SET NOT NULL,
    ADD CONSTRAINT weight_check CHECK (weight > 0),
    ADD CONSTRAINT cost_check CHECK (cost > 0);

ALTER TABLE Ingredient
    ALTER COLUMN name SET NOT NULL,
    ADD CONSTRAINT name_check CHECK (name ~ '^[a-zA-Zа-яА-ЯёЁ0-9 ,."-]+$'),
    ALTER COLUMN inStock SET NOT NULL,
    ADD FOREIGN KEY (supplierId) REFERENCES Supplier(supplierId);

ALTER TABLE Visitor
    ALTER COLUMN name SET NOT NULL,
    ADD CONSTRAINT name_check CHECK (name ~ '^[a-zA-Zа-яА-ЯёЁ0-9 ,."-]+$'),
    ADD CONSTRAINT phoneNumber_check CHECK (phoneNumber ~ '^\+375[0-9]{9}$');

ALTER TABLE "Order"
    ALTER COLUMN totalCost SET NOT NULL,
    ADD CONSTRAINT totalCost_check CHECK (totalCost >= 0),
    ADD CONSTRAINT numberOfGuests_check CHECK (numberOfGuests >= 0),
    ALTER COLUMN orderDateTime SET NOT NULL,
    ADD FOREIGN KEY (visitorId) REFERENCES Visitor(visitorId),
    ADD FOREIGN KEY (employeeId) REFERENCES Employee(employeeId);

ALTER TABLE EmployeePosition
    ADD FOREIGN KEY (employeeId) REFERENCES Employee(employeeId),
    ADD FOREIGN KEY (positionId) REFERENCES Position(positionId);

ALTER TABLE DishIngredient
    ADD FOREIGN KEY (dishId) REFERENCES Dish(dishId),
    ADD FOREIGN KEY (ingredientId) REFERENCES Ingredient(ingredientId);

ALTER TABLE OrderDish
    ADD FOREIGN KEY (orderId) REFERENCES "Order"(orderId),
    ADD FOREIGN KEY (dishId) REFERENCES Dish(dishId);



