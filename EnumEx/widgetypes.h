#ifndef WIDGETYPES_H
#define WIDGETYPES_H

#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include <tuple>

#include "enum.h"
#include "monad.h"
#include <fstream>
#include <QMainWindow>
#include <QApplication>
#include <QComboBox>
#include <QCheckBox>
#include <map>
#include <cint.h>
#include <typeinfo>
#include <type_traits>
#include "fileops.h"
#include <sstream>


namespace Ui {
class MainWindow;
}

//template<auto u>
//using cint = typename std::integral_constant<decltype(u), u>;


enum TickTypeDef{Active = 5, Deactive};

enum ColorTypeDef {Red = 25, Green, Blue = 30, Orange};

template<typename TUPLE>
class WidgetTypes
{

public:


    const TUPLE info;




    WidgetTypes(const TUPLE Info) : info(Info) {

    }


    WidgetTypes operator = (const TUPLE Info)
    {
        info = Info;

        return *this;
    }

    ~WidgetTypes(){

    }

    template<typename ENUMTYPE, size_t i = 0>
    constexpr inline std::string infoOf()
    {
        const auto sz = std::tuple_size_v<TUPLE>;

        if constexpr(std::is_same_v<ENUMTYPE, std::tuple_element_t<i, TUPLE>>)
           return std::get<i+1>(info);

        if constexpr(i < sz-1)
           infoOf<ENUMTYPE, i+1>();

    }


    operator std::vector<std::string> ()
    {
        std::vector<std::string> vec;
        populateVector<0>(vec);

        return vec;

    }

    template<typename INTEGRAL>
    operator std::map<INTEGRAL, std::string>()
    {
        std::map<INTEGRAL, std::string> Map;
        populateMap<INTEGRAL, 0>(Map);

        return Map;
    }

    WidgetTypes operator = (int Enum)
    {
        return *this;
    }

    template<typename ENUMTYPE, typename W>
    constexpr void bind(W* Widget)
    {

        if constexpr(std::is_same<W, QComboBox>::value)
            populateComboBox<ENUMTYPE, 0>(Widget);
        if constexpr(std::is_same<W, QCheckBox>::value)
            populateCheckBox<ENUMTYPE, 0>(Widget);

    }



    template<typename ENUMTYPE, size_t i = 0>
    constexpr inline void populateComboBox(QComboBox* ComboBox)
    {

        const auto sz = std::tuple_size_v<TUPLE>;

        if constexpr(!std::is_same_v<const char *, std::tuple_element_t<i, TUPLE>>)
             if constexpr(std::is_same<decltype(std::get<i>(info).cint_value), const ENUMTYPE>::value)
                     ComboBox->addItem(std::get<i + 1>(info));


        if constexpr(i < sz-1)
            populateComboBox<ENUMTYPE, i+1>(ComboBox);



    }


    template<typename ENUMTYPE, size_t i = 0>
    constexpr inline void populateCheckBox(QCheckBox *CheckBox)
    {



    }



    template<size_t i>
    constexpr inline void populateVector(std::vector<std::string>& Vector)
    {

        const auto sz = std::tuple_size_v<TUPLE>;

        if constexpr(!(std::is_same<const char *, std::tuple_element_t<i, TUPLE>>::value))
            if constexpr(std::is_same<decltype(std::get<i>(info).cint_value), const ColorTypeDef>::value)
                Vector.push_back(std::get<i + 1>(info));

        if constexpr (i < sz-1)
            populateVector<i+1>(Vector);

    }

    template<typename INTEGRAL, size_t i>
    constexpr inline void populateMap(std::map<INTEGRAL, std::string>& Map)
    {
        ColorTypeDef enum_color;

        const auto sz = std::tuple_size_v<TUPLE>;
        if constexpr(!(std::is_same<const char *, std::tuple_element_t<i, TUPLE>>::value))
        {
            if constexpr(std::is_same<decltype(std::get<i>(info).cint_value), const ColorTypeDef>::value)
            {
                auto A = std::get<i>(info);
                Map[INTEGRAL(A)] = std::get<i + 1>(info);
            }
        }
        if constexpr (i < sz-1)
        {

            populateMap<INTEGRAL, i+1>(Map);

        }
    }



    template <typename PERSISTENCE, size_t i = 0>
    constexpr inline void setPersistancy(PERSISTENCE Persistence)
    {

        const auto sz = std::tuple_size_v<PERSISTENCE>;
        if constexpr(std::is_same<QComboBox *, std::tuple_element_t<i, PERSISTENCE>>::value)
        {
            if constexpr (i < sz-1)
            {
                if constexpr(std::is_same<ColorTypeDef, std::tuple_element_t<i+1, PERSISTENCE>>::value)
                {
                    bind<ColorTypeDef>(std::get<i>(Persistence));
                }
            }
            else
            {
                std::string token;
                std::string ret = FileOps::readFile();
                std::istringstream ss(ret);
                std::getline(ss, token, ',');
                std::map<int, std::string> M = *this;
                std::get<i>(Persistence)->setCurrentText((M.find(atoi(token.c_str()))->second).c_str());
            }
        }

        if constexpr(std::is_same<QCheckBox *, std::tuple_element_t<i, PERSISTENCE>>::value)
        {
            if constexpr (i < sz-1)
            {
                if constexpr(std::is_same<TickTypeDef, std::tuple_element_t<i+1, PERSISTENCE>>::value)
                {
                    bind<TickTypeDef>(std::get<i>(Persistence));
                }     
            }
            else
            {

                std::get<i>(Persistence)->setCheckState(Qt::CheckState::Checked);

                std::string token;
                std::string ret = FileOps::readFile();
                std::istringstream ss(ret);
                std::getline(ss, token, ',');
                std::getline(ss, token, ',');

                int a = (int)token.c_str()[0] -'0';

//                std::cout << a << std::endl;

                std::get<i>(Persistence)->setText(infoOf<cint<TickTypeDef(a)>>().c_str());


            }
        }

        if constexpr (i < sz-1)
        {
            setPersistancy<PERSISTENCE, i+1>(Persistence);
        }
    }



    template <typename PERSISTENCE>
    constexpr inline void setCurrentContext(PERSISTENCE Persistence)
    {


    }

};

#endif // COLORTYPE_H
