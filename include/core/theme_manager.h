#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <QObject>
#include <QString>

/**
 * @brief Перечисление доступных тем приложения
 */
enum class ThemeType {
    Light,
    Dark,
    Wildberries
};

/**
 * @brief ThemeManager - синглтон для управления темами приложения
 * 
 * Загружает QSS из ресурсов и применяет их глобально.
 * Также настраивает QPalette для корректного отображения.
 */
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Получить экземпляр менеджера тем (Meyers Singleton)
     */
    static ThemeManager& instance();

    // Запрет копирования и перемещения
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    ThemeManager(ThemeManager&&) = delete;
    ThemeManager& operator=(ThemeManager&&) = delete;

    /**
     * @brief Применить тему к приложению
     * @param type Тип темы для применения
     */
    void applyTheme(ThemeType type);

    /**
     * @brief Получить текущую активную тему
     * @return Текущий тип темы
     */
    ThemeType currentTheme() const;

    /**
     * @brief Загрузить сохранённую тему из QSettings
     */
    void loadSavedTheme();

    /**
     * @brief Получить название темы для отображения
     * @param type Тип темы
     * @return Локализованное название темы
     */
    static QString themeDisplayName(ThemeType type);

    /**
     * @brief Преобразовать ThemeType в строку для сохранения
     */
    static QString themeToString(ThemeType type);

    /**
     * @brief Преобразовать строку обратно в ThemeType
     */
    static ThemeType stringToTheme(const QString& str);

signals:
    /**
     * @brief Сигнал об изменении темы
     * @param newTheme Новая применённая тема
     */
    void themeChanged(ThemeType newTheme);

private:
    explicit ThemeManager(QObject* parent = nullptr);
    ~ThemeManager() override = default;

    /**
     * @brief Загрузить QSS файл из ресурсов
     * @param resourcePath Путь к ресурсу (например ":/styles/style_light.qss")
     * @return Содержимое QSS или пустая строка при ошибке
     */
    QString loadStyleSheet(const QString& resourcePath);

    /**
     * @brief Настроить QPalette для указанной темы
     * @param type Тип темы
     */
    void setupPalette(ThemeType type);

    /**
     * @brief Сохранить выбранную тему в QSettings
     * @param type Тип темы для сохранения
     */
    void saveTheme(ThemeType type);

    ThemeType m_currentTheme;
};

#endif // THEME_MANAGER_H