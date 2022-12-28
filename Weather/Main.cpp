#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#pragma comment(lib, "ws2_32")

// Определить структуру для хранения данных о погоде
struct WeatherData {
	std::string location; // Местоположение
	double temperature; // Температура
	double humidity; // Влажность
	double wind_speed; // Скорость ветра
};

// Функция для создания строки запроса для запроса к API
std::string build_query_string(const std::string& api_key, const std::string& location) {
	std::string query; // Строка запроса
	query = "q=" + location + "&units=metric&appid=" + api_key; // Формирование строки запроса
	return query; // Вернуть строку запроса
}

// Функция для заполнения структуры WeatherData данными из API
void get_weather_data(WeatherData& data, const std::string& api_key, const std::string& location) {
	// Установить конечную точку API и ключ API
	std::string endpoint = "https://api.openweathermap.org/data/2.5/weather";
	std::string query_string = build_query_string(api_key, location); // Сформировать строку запроса

	// Сформировать строку запроса
	std::string request;
	request += "GET " + endpoint + "?" + query_string + "HTTP/1.1\r\n"
		+ "Host: api.openweathermap.org\r\n"
		+ "User-Agent: C++ HTTP Client\r\n"
		+ "Accept: application/json\r\n"
		+ "Connection: close\r\n\r\n";

	// Открыть поток к конечной точке API
	std::string server = "api.openweathermap.org";
	std::fstream stream(("https://" + server + endpoint + "?" + query_string).c_str(), std::ios::in);

	// Прочитать ответ из потока
	std::string response;
	std::getline(stream, response, '\0');

	// Разделить ответ на заголовок и тело
	std::string header = response.substr(0, response.find("\r\n\r\n"));
	std::string body = response.substr(response.find("\r\n\r\n") + 4);

	// Разбор тела ответа
	size_t pos = 0;
	std::string token;
	std::string key;
	std::string value;

	// Извлечение данных из объекта JSON и заполнение структуры WeatherData
	while ((pos = body.find(":")) != std::string::npos) {
		key = body.substr(0, pos); // Извлечение ключа
		body.erase(0, pos + 1); // Удаление ключа из тела ответа
		if (key == "\"name\"") {
			// Извлечение местоположения
			pos = body.find("\"");
			token = body.substr(pos + 1, body.find("\"", pos + 1) - pos - 1);
			data.location = token;
		}
		else if (key == "\"temp\"") {
			// Извлечение температуры
			pos = body.find(",");
			token = body.substr(0, pos);
			data.temperature = std::stod(token);
		}
		else if (key == "\"humidity\"") {
			// Извлечение влажности
			pos = body.find(",");
			token = body.substr(0, pos);
			data.humidity = std::stod(token);
		}
		else if (key == "\"speed\"") {
			// Извлечение скорости ветра
			pos = body.find(",");
			token = body.substr(0, pos);
			data.wind_speed = std::stod(token);
		}
		body.erase(0, pos + 1); // Удаление значения из тела ответа
	}
}

void write_weather_data_to_file(const WeatherData& data, const std::string& filename) {
	std::ofstream file(filename); // Открыть файл для записи
	if (file.is_open()) { // Если файл успешно открыт
		file << data.location << '\n'; // Записать местоположение
		file << data.temperature << '\n'; // Записать температуру
		file << data.humidity << '\n'; // Записать влажность
		file << data.wind_speed; // Записать скорость ветра

		file.close(); // Закрыть файл
	}
	else {
		std::cout << "Не удалось открыть файл для записи." << std::endl;
	}
}

int main() {
	setlocale(LC_ALL, "ru"); // Я это не часто использую, так как в моей консоли кирилица выглядит так "?????? ???" - привет МИР

	WeatherData A; // Создать структуру данных о погоде

	std::string city;
	std::cin >> city;
	city += ", ua";
	get_weather_data(A, "b7416bbf98cf92c12098f1cb7be49211", city); // Получить данные о погоде для Одессы, Украина

	// Вывести данные о погоде
	std::cout
		<< "\n\tМестоположение: " << A.location << std::endl
		<< "\tТемпература: " << A.temperature << std::endl
		<< "\tСкорость ветра: " << A.wind_speed << std::endl
		<< "\tВлажность: " << A.humidity;

	write_weather_data_to_file(A, "file");

	return 0;
}