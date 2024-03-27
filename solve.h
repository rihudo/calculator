#ifndef SOLVE_H
#define SOLVE_H

#include <string>
#include <memory>
#include <stack>
#include <unordered_map>
#include <QObject>
#include <QString>
#include <type_traits>

using std::string;
using std::unique_ptr;
using std::stack;
using std::unordered_map;

enum NUM_TYPE
{
    INTEGER,
    FLOAT
};

class Number
{
public:
    Number(NUM_TYPE _type) : type(_type) {}

    NUM_TYPE getType()
    {
        return type;
    }

    virtual string getString() = 0;
    virtual ~Number() {}

protected:
    NUM_TYPE type;
};

template <typename T>
class NumberValue : public Number
{
public:
    NumberValue(NUM_TYPE _type, T _value) : Number(_type), value(_value)
    {}

    virtual string getString() override
    {
        string text(std::to_string(value));
        if (string::npos != text.find('.'))
        {
            while (!text.empty() && text.back() == '0' || text.back() == '.')
            {
                text.pop_back();
            }
        }
        return text;
    }

    T getValue()
    {
        return value;
    }

private:
    T value;
};

template<typename T>
static unique_ptr<T> getValue(Number* _ptr)
{
    NumberValue<T>* ptr = dynamic_cast<NumberValue<T>*>(_ptr);
    if (!ptr)
    {
        return unique_ptr<T>();
    }
    return std::make_unique<T>(ptr->getValue());
}

static unique_ptr<Number> calculate(stack<unique_ptr<Number>>& num_stack, char opt)
{
    if (num_stack.size() < 2)
    {
        return unique_ptr<Number>();
    }
    unique_ptr<Number> right(std::move(num_stack.top()));
    num_stack.pop();
    unique_ptr<Number> left(std::move(num_stack.top()));
    num_stack.pop();
    auto left_num = left->getType() == NUM_TYPE::FLOAT ? *getValue<float>(left.get()) : *getValue<int>(left.get());
    auto right_num = right->getType() == NUM_TYPE::FLOAT ? *getValue<float>(right.get()) : *getValue<int>(right.get());
    switch (opt)
    {
        case '+':
        {
            auto ret = left_num + right_num;
            return std::make_unique<NumberValue<decltype(ret)>>(std::is_same<decltype(ret), \
                float>::value ? NUM_TYPE::FLOAT : NUM_TYPE::INTEGER, ret);
        }
        break;

        case'-':
        {
            auto ret = left_num - right_num;
            return std::make_unique<NumberValue<decltype(ret)>>(std::is_same<decltype(ret), \
                float>::value ? NUM_TYPE::FLOAT : NUM_TYPE::INTEGER, ret);
        }
        break;

        case 'x':
        {
            auto ret = left_num * right_num;
            return std::make_unique<NumberValue<decltype(ret)>>(std::is_same<decltype(ret),
                float>::value ? NUM_TYPE::FLOAT : NUM_TYPE::INTEGER, ret);
        }
        break;

        case '/':
        {
            auto ret = left_num / right_num;
            return std::make_unique<NumberValue<decltype(ret)>>(std::is_same<decltype(ret),
                float>::value ? NUM_TYPE::FLOAT : NUM_TYPE::INTEGER, ret);
        }
        break;

        default:
            return unique_ptr<Number>();
    }
    return unique_ptr<Number>();
}

static string getResult(const string& str)
{
    stack<unique_ptr<Number>> num_stack;
    stack<char> opt_stack;
    unordered_map<char, short> opt_map{{'+', 0}, {'-', 0}, {'x', 1}, {'/', 1}};

    auto convert = [&num_stack](string& num_str)
    {
        if (!num_str.empty())
        {
            if (string::npos != num_str.find('.'))
                num_stack.emplace(std::make_unique<NumberValue<float>>(NUM_TYPE::FLOAT, std::stof(num_str)));
            else
                num_stack.emplace(std::make_unique<NumberValue<int>>(NUM_TYPE::INTEGER, std::stoi(num_str)));
        }
        num_str.clear();
    };

    string tmp;
    for (const char& ch : str)
    {
        if (ch == ' ')
            continue;

        if (ch == '(')
        {
            opt_stack.push(ch);
            continue;
        }

        if (ch == ')')
        {
            convert(tmp);
            while (!opt_stack.empty())
            {
                if (opt_stack.top() == '(')
                {
                    opt_stack.pop();
                    break;
                }
                num_stack.emplace(calculate(num_stack, opt_stack.top()));
                opt_stack.pop();
            }
            continue;
        }

        if (opt_map.count(ch))
        {
            if (ch == '-' && tmp.empty())
            {
                tmp.push_back(ch);
                continue;
            }
            convert(tmp);
            while (!opt_stack.empty() && opt_stack.top() != '(' && opt_map.at(ch) < opt_map.at(opt_stack.top()))
            {
                num_stack.emplace(calculate(num_stack, opt_stack.top()));
                opt_stack.pop();
            }
            opt_stack.push(ch);
        }
        else if (ch >= '0' && ch <= '9' || ch == '.')
        {
            tmp.push_back(ch);
        }
    }
    convert(tmp);
    while (!opt_stack.empty() && num_stack.size() >= 2)
    {
        num_stack.emplace(calculate(num_stack, opt_stack.top()));
        opt_stack.pop();
    }

    if (num_stack.empty())
    {
        return "ERROR";
    }

    return num_stack.top()->getString();
}

class solve : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isResult READ isResult WRITE setIsResult NOTIFY isResultChanged)

public:
    explicit solve(QObject *parent = nullptr);
    bool isResult();
    void setIsResult(bool);

    Q_INVOKABLE QString num_calculate(const QString& str);

signals:
    void isResultChanged();

private:
    bool m_isResult;
};

#endif // SOLVE_H
