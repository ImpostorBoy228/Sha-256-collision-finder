#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include <chrono>
#include <map>
#include <openssl/sha.h>
#include <sstream>
#include <cstdint> // Для uint8_t

// Функция для вычисления SHA256 хеша строки и возврата в виде hex-строки
std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

int main() {
    // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255); // Используем uint8_t вместо char

    // Map для хранения хеш -> входная строка (для поиска коллизий)
    std::map<std::string, std::string> hash_map;

    // Счетчики и таймеры
    unsigned long long iterations = 0;
    auto start_time = std::chrono::steady_clock::now();
    auto last_update = start_time;
    unsigned long long last_iterations = 0;

    // Длина сообщения (произвольная, можно изменить)
    const size_t msg_length = 16; // 16 байт случайных сообщений

    std::cout << "Запуск поиска коллизий SHA256..." << std::endl;

    while (true) {
        // Генерация случайного ввода
        std::string input(msg_length, 0);
        for (char& c : input) {
            c = static_cast<char>(dis(gen)); // Преобразуем uint8_t в char
        }

        // Вычисление хеша
        std::string hash = sha256(input);
        iterations++;

        // Проверка на коллизию
        auto it = hash_map.find(hash);
        if (it != hash_map.end() && it->second != input) {
            // Коллизия найдена!
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = now - start_time;
            std::cout << "\n\nКОЛЛИЗИЯ НАЙДЕНА после " << iterations << " итераций за " 
                      << elapsed.count() << " секунд!" << std::endl;
            std::cout << "Вход 1 (hex): " << std::endl;
            for (char c : it->second) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                          << (static_cast<unsigned char>(c) & 0xFF) << " ";
            }
            std::cout << std::endl;
            std::cout << "Вход 2 (hex): " << std::endl;
            for (char c : input) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                          << (static_cast<unsigned char>(c) & 0xFF) << " ";
            }
            std::cout << std::endl;
            std::cout << "Общий хеш: " << hash << std::endl;
            // Продолжить поиск или прервать, если нужно остановиться
            // break;
        } else {
            // Сохраняем хеш и вход
            hash_map[hash] = input;
        }

        // Обновление отображения каждую секунду
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> since_last = now - last_update;
        if (since_last.count() >= 1.0) {
            unsigned long long ips = (iterations - last_iterations) / since_last.count();
            std::cout << "\rИтераций: " << iterations << " | Хешей/сек: " << ips 
                      << " | Сохранено хешей: " << hash_map.size() << std::flush;

            last_update = now;
            last_iterations = iterations;
        }
    }

    return 0;
}
