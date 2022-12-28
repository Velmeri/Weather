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

	// Вывести заголовок и тело
	std::cout << "Заголовок:\n" << header << "\n\nТело:\n" << body << std::endl;
}

int main() {
	setlocale(LC_ALL, "ru"); // Я это не часто использую, так как в моей консоли кирилица выглядит так "?????? ???" - привет МИР

	WeatherData A; // Создать структуру данных о погоде
	get_weather_data(A, "b7416bbf98cf92c12098f1cb7be49211", "Odessa, ua"); // Получить данные о погоде для Одессы, Украина

	// Вывести данные о погоде
	std::cout
		<< "\n\tМестоположение: " << A.location << std::endl
		<< "\tТемпература: " << A.temperature << std::endl
		<< "\tСкорость ветра: " << A.wind_speed << std::endl
		<< "\tВлажность: " << A.humidity;

	return 0;
}