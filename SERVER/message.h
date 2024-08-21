#pragma once
#include <iostream>
#include <string>
#include <vector>

class Message
{
    std::string m_time;   //переменная текущего времени
    std::string m_nameOf; //переменная имени отправитела
    std::string m_nameTo; //переменная имени получателя
    std::string m_text;   //переменная текста сообщения
public:
    Message();
    Message(std::string& time, std::string& nameOf, std::string& nameTo, std::string& text);
    std::string Time(); //получение текущего времени
    std::vector<Message> array; //массив записи сообщений
    void setData();//установка данных персональных сообщений
    void getData(); //получение и вывод введенных в массив данных
    void createMessages(); //создание персональных сообщений
    static int countRecords(const std::string name_file); //подсчет числа строк в текстовом файле
    void readPersonalMessages(); //чтение персональных сообщений
    friend std::ostream& operator<< (std::ostream& out, const Message& z);
    friend std::istream& operator>> (std::istream& in, Message& z);
    ~Message();
};