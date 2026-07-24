# MarketPlace

**Версия:** 1.0.0 | **Стандарт:** C++17 | **Фреймворк:** Qt 6

Кроссплатформенное десктопное приложение-маркетплейс с управлением товарами, корзиной, профилем пользователя и тремя темами оформления.

---

## Функциональность

### Покупатель
- Регистрация и авторизация (username + password)
- Просмотр каталога: сетка карточек 4 колонки, фильтр по категориям
- Поиск товаров по названию, описанию и бренду (регистронезависимый)
- Добавление в корзину, изменение количества (до остатка на складе), удаление
- Оформление заказа: списание с баланса, уменьшение остатков
- Профиль: редактирование email/телефона/адреса, пополнение баланса (до 1 000 000 за транзакцию)

### Администратор
- Регистрация с секретным кодом `ADMIN2024SECRET`
- Панель управления: таблица всех товаров + форма редактирования
- Добавление, изменение, удаление товаров
- Валидация: название обязательно, цена > 0

### Темы оформления
- Светлая (`#4a90d9` синий акцент)
- Тёмная (`#1e1e1e` фон, `#3794ff` синий акцент)
- Wildberries (`#cb11ab` фиолетово-розовый акцент)
- Переключение без перезапуска, сохранение в QSettings

---

## Архитектура

```
├── include/
│   ├── core/
│   │   ├── database.h          # Хранилище + хэш-индексы
│   │   └── theme_manager.h     # Синглтон управления QSS
│   ├── models/
│   │   ├── cart.h              # Корзина (map<int, CartItem>)
│   │   ├── product.h           # Товар (13 полей)
│   │   └── user.h              # Пользователь (Customer/Admin)
│   └── ui/
│       ├── adminpanel.h        # CRUD товаров (QTableWidget + форма)
│       ├── cartwidget.h        # Список товаров в корзине
│       ├── loginwindow.h       # Форма входа
│       ├── mainwindow.h        # Оркестратор (QStackedWidget)
│       ├── productcard.h       # Карточка товара 220x320
│       ├── profilewidget.h     # Редактирование профиля
│       └── registerwindow.h    # Форма регистрации с валидацией
├── src/                        # Реализации (.cpp)
├── forms/mainwindow.ui         # Qt Designer (минимальный)
├── resources/styles/
│   ├── style_light.qss         # 989 строк
│   ├── style_dark.qss          # 991 строка
│   └── style_wb.qss            # 987 строк
└── wild.pro                    # qmake конфигурация
```

---

## Модели данных

### User (users.txt)
```
id|username|password|email|phone|address|balance|role
1|john|pass123|john@mail.com|||1000.0|0
```
- `role`: 0 = Customer, 1 = Admin
- pipe-разделитель, 8 полей
- Начальный баланс при регистрации: 1000.0

### Product (products.txt)
```
id|name|description|category|brand|price|oldPrice|discount|stock|rating|reviewCount|imageUrl|isAvailable
1|iPhone 15 Pro Max 256GB|Flagship...|Electronics|Apple|116991|129990|10|50|4.9|1523||1
```
- pipe-разделитель, 13 полей
- `discount` от 0 до 99, `stock` >= 0
- `isAvailable`: 0 или 1, `isAvailable()` возвращает `m_isAvailable && m_stock > 0`
- `getFinalPrice()` возвращает `m_price` (уже со скидкой)
- При `setDiscount(d)` > 0: `m_oldPrice` = старая цена, `m_price` = `oldPrice * (100 - d) / 100`

### Cart
- `std::map<int, CartItem>` — ключ = productId
- `CartItem`: `Product` + `quantity`
- Методы: `getTotalPrice()`, `getTotalDiscount()`, `getTotalItemsCount()`, `getItem(int)`
- Копирование запрещено, разрешено перемещение

---

## Database (ядро)

- `data/` директория, файлы `users.txt` и `products.txt`
- При старте: создание директории, загрузка файлов, автоинициализация 14 товаров если пусто
- Хэш-индексы: `unordered_map<int, size_t>` по ID, `unordered_map<string, size_t>` по username, `unordered_set<string>` emails
- Флаг `m_dirty` — запись на диск только при изменениях
- При `flush()` и в деструкторе — сохранение
- Поиск: `searchProducts(query)` — регистронезависимый contains по name/description/brand
- Категории: `getCategories()` — уникальные, отсортированные

### 14 предустановленных товаров

| Товар | Категория | Цена | Скидка |
|-------|-----------|------|--------|
| iPhone 15 Pro Max 256GB | Electronics | 116 991 | 10% |
| Samsung Galaxy S24 Ultra | Electronics | 101 991 | 15% |
| MacBook Air M3 15" | Electronics | 149 990 | 0% |
| Sony WH-1000XM5 | Electronics | 26 392 | 20% |
| Men's Winter Jacket | Clothing | 6 293 | 30% |
| Women's Summer Dress | Clothing | 2 242 | 25% |
| Nike Air Max Sneakers | Footwear | 11 041 | 15% |
| Xiaomi Robot Vacuum | Home & Garden | 22 491 | 10% |
| DeLonghi Coffee Machine | Home & Garden | 45 990 | 0% |
| L'Oreal Cosmetics Set | Beauty | 2 394 | 40% |
| Chanel No 5 Perfume | Beauty | 8 990 | 0% |
| LEGO Star Wars | Kids | 14 240 | 5% |

---

## ThemeManager

Meyers Singleton — `static ThemeManager& instance()`

- `applyTheme(ThemeType)` — загружает QSS из ресурсов через `qApp->setStyleSheet()`, настраивает `QPalette`, сохраняет в `QSettings`
- `loadSavedTheme()` — читает сохранённую тему из `QSettings`
- Сигнал `themeChanged(ThemeType)` — MainWindow подписывается для синхронизации комбобокса
- Три темы: `Light`, `Dark`, `Wildberries` (по умолчанию)

---

## UI / Навигация

### MainWindow (829 строк)
- `QStackedWidget` — переключение страниц
- **Страницы**: Login → Register → MainPage → Cart → Profile → Settings → Admin
- **Боковая панель** (280px): логотип, баланс, кнопки (Catalog/Cart/Profile/Settings/Admin/Sign Out), `QButtonGroup` с exclusive выбором
- **Шапка** (70px): поиск (QLineEdit, Enter → onSearch), кнопки Cart (с бейджем количества), Profile, Admin, Settings
- **Категории** (240px): QListWidget, "All Products" + динамические категории
- **Сетка товаров**: QGridLayout в QScrollArea, 4 колонки, spacing 24px

### ProductCard (220x320px)
- Иконка категории: `[E]`, `[C]`, `[F]`, `[H]`, `[B]`, `[K]`, `[S]`, `[A]`, `[P]`
- Бейдж скидки `-X%`, бренд, название (2 строки), рейтинг `* X.X (N)`, цена, старая цена (crossed)
- Кнопка "Add to Cart" — `emit addToCartClicked(product)`
- Клик по карточке — `emit productClicked(product)`
- Тень: `QGraphicsDropShadowEffect` переиспользуется, blur 15 → 20 при наведении

### LoginWindow
- Форма 400px с тенью: username, password, "Sign In", "or", "Create Account"
- `authenticateUser()` → `emit loginSuccess(User*)` или "Invalid username or password"
- Enter в password → вход

### RegisterWindow
- Форма 450px: username, email, password, confirm password, checkbox admin + secret code
- Валидация: username ≥ 3 символа, только `[a-zA-Z0-9_]`, email regex, password ≥ 4, совпадение паролей
- Admin code: `ADMIN2024SECRET`, `isValidAdminCode()` статический метод

### CartWidget
- Список элементов: иконка, название, цена, QSpinBox (1..stock), общая цена, кнопка "X" (красная)
- Итоги: скидка (зелёным), общая сумма, "Checkout"
- Пустое состояние: "Your cart is empty"

### ProfileWidget
- Аватар `[U]` или `[A]` (100px, круг), роль, баланс
- Поля: username (readonly), email, phone, address
- "Add Funds" `[amount] [Add]` — макс 1 000 000
- "Save Changes" → `updateUser()`, "Sign Out" → `emit logoutClicked()`

### AdminPanel
- Левая панель: QTableWidget (ID, Name, Category, Brand, Price, Stock, Discount), 7 колонок
- Правая панель: QFormLayout — Name, Description (QTextEdit), Brand, Category (QComboBox: Electronics/Clothing/Footwear/Home & Garden/Beauty/Kids/Sports/Auto), Price (0..9 999 999), Stock (0..99 999), Discount (0..99%)
- Кнопки: Add (зелёный), Update (синий), Delete (красный), Clear Form
- Выбор строки → заполнение формы, "Update" и "Delete" активируются
- `emit productsChanged()` → MainWindow.refreshCatalog()

---

## Сигналы и слоты (межкомпонентное взаимодействие)

```
LoginWindow::loginSuccess(User*)        → MainWindow::onLoginSuccess
LoginWindow::registerClicked            → MainWindow::showRegister
RegisterWindow::registerSuccess         → MainWindow::onRegisterSuccess
RegisterWindow::backToLogin             → MainWindow::showLogin
ProductCard::addToCartClicked(Product)  → MainWindow::onAddToCart
CartWidget::checkoutClicked             → MainWindow::onCheckout
CartWidget::cartUpdated                 → MainWindow::updateCartBadge
ProfileWidget::logoutClicked            → MainWindow::logout
ProfileWidget::balanceChanged           → MainWindow (lambda — обновить баланс в sidebar)
AdminPanel::productsChanged             → MainWindow::refreshCatalog
ThemeManager::themeChanged(ThemeType)   → MainWindow::onThemeApplied (синхронизация комбобокса)
```

---

## Сборка и запуск

**Требования:** Qt 6.0+, C++17, любой C++ компилятор

```bash
# qmake
mkdir build && cd build
qmake ../wild.pro
make -j4          # или nmake / mingw32-make
```

**Qt Creator:** открыть `wild.pro`, выбрать kit, Ctrl+R.

---

## Формат данных

Пользователи и товары хранятся в `data/` как pipe-разделённые строки (по одной записи на строку). Это позволяет вручную редактировать данные в любом текстовом редакторе между запусками приложения.

```
data/
├── users.txt        # 8 полей через |
└── products.txt     # 13 полей через |
```

---

## Технические детали

- Move-семантика: `User`, `Product`, `Cart`, `Database` — все поддерживают перемещение
- Запрет копирования: `Cart`, `Database` — `delete` copy constructor/assignment
- RAII: `ThemeManager` Meyers Singleton, `QObject* parent` для автоматического удаления
- `const`-корректность: геттеры возвращают `const&`, методы-сеттеры с перегрузкой `std::string&&`
- `noexcept` на move-конструкторах, операциях с балансом, геттерах
- Инкапсуляция: приватные поля с префиксом `m_`
- Категории — enum через `QComboBox` в AdminPanel (8 предопределённых), для фильтрации — динамический список из `getCategories()`
- Поиск — регистронезависимый, по трём полям (name, description, brand)
- Корзина — непереносимая между сессиями (не сохраняется в файл)

---

## Roadmap (из кода)

- [ ] SQLite вместо текстовых файлов
- [ ] Загрузка изображений товаров
- [ ] Хэширование паролей (bcrypt)
- [ ] Аналитика для администратора