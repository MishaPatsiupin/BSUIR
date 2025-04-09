-- Таблица Поставщик
CREATE TABLE Supplier (
    supplierId SERIAL PRIMARY KEY,
    companyName NVARCHAR(100),
    contactInformation NVARCHAR(255),
    reliabilityRating INT,
    productCategory NVARCHAR(100)
);

-- Таблица Должность
CREATE TABLE Position (
    positionId SERIAL PRIMARY KEY,
    name NVARCHAR(100),
    salary DECIMAL(10, 2),
    workSchedule VARCHAR(100),
    responsibilities NTEXT
);

-- Таблица Сотрудник
CREATE TABLE Employee (
    employeeId SERIAL PRIMARY KEY,
    fullName NVARCHAR(100),
    phonePhone VARCHAR(15),
    email VARCHAR(100),
    hireDate DATE,
    contractEndDate DATE
);

-- Таблица Блюдо
CREATE TABLE Dish (
    dishId SERIAL PRIMARY KEY,
    name NVARCHAR(100),
    weight DECIMAL(10, 2),
    nutritionalValue NVARCHAR(100),
    cost NUMERIC(10, 2),
    category NVARCHAR(100)
);

-- Таблица Ингредиент
CREATE TABLE Ingredient (
    ingredientId SERIAL PRIMARY KEY,
    name NVARCHAR(100),
    unitOfMeasurement NVARCHAR(50),
    inStock DECIMAL(10, 2),
    expirationDate DATE,
    supplierId INT,
    --FOREIGN KEY (supplierId) REFERENCES Supplier(supplierId)
);

-- Таблица Посетитель
CREATE TABLE Visitor (
    visitorId SERIAL PRIMARY KEY,
    name NVARCHAR(100),
    birthDate DATE,
    phoneNumber VARCHAR(15),
    preferences NTEXT
);

-- Таблица Заказ
CREATE TABLE "Order" (
    orderId SERIAL PRIMARY KEY,
    status NVARCHAR(50),
    totalCost DECIMAL(10, 2),
    tableNumber INT,
    orderDateTime TIMESTAMP,
    visitorId INT,
    employeeId INT,
    --FOREIGN KEY (visitorId) REFERENCES Visitor(visitorId),
    --FOREIGN KEY (employeeId) REFERENCES Employee(employeeId)
);

-- Таблица Должность-Сотрудник (связь многие-ко-многим)
CREATE TABLE EmployeePosition (
    employeeId INT,
    positionId INT,
    PRIMARY KEY (employeeId, positionId),
    --FOREIGN KEY (employeeId) REFERENCES Employee(employeeId),
    --FOREIGN KEY (positionId) REFERENCES Position(positionId)
);

-- Таблица Блюдо-Ингредиент (связь многие-ко-многим)
CREATE TABLE DishIngredient (
    dishId INT,
    ingredientId INT,
    PRIMARY KEY (dishId, ingredientId),
    --FOREIGN KEY (dishId) REFERENCES Dish(dishId),
    --FOREIGN KEY (ingredientId) REFERENCES Ingredient(ingredientId)
);

-- Таблица Блюдо-Заказ (связь многие-ко-многим)
CREATE TABLE OrderDish (
    orderId INT,
    dishId INT,
    PRIMARY KEY (orderId, dishId),
    --FOREIGN KEY (orderId) REFERENCES "Order"(orderId),
    --FOREIGN KEY (dishId) REFERENCES Dish(dishId)
);