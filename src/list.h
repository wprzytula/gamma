/** @file
 * Interfejs listy przechowującej wskaźniki jako wartości.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 27.03.2020
 */

#ifndef GAMMA_LIST_H
#define GAMMA_LIST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>


/**
 * Typ pojedynczego elementu listy.
 */
typedef struct list_elem list_elem_t;


/**
 * Struktura pojedynczego elementu listy.
 */
struct list_elem {
    void *value;            ///< przechowywana wartość - wskaźnik
    list_elem_t *left;      ///< wskaźnik na element wcześniej na liście
    list_elem_t *right;     ///< wskaźnik na element później na liście
    };

/**
 * Struktura listy przechowującej wskaźniki jako wartości.
 */
typedef struct {
    uint32_t logical_size;    ///< liczba elementów na liście
    list_elem_t *left_guard;  ///< strażnik początku listy (pusty element)
    list_elem_t *right_guard; ///< strażnik końca listy (pusty element)
} list_t;

/** @brief Sprawdza czy lista jest pusta.
 * Sprawdza, czy pomiędzy strażnikami listy @p list nie ma żadnych elementów.
 * Zakłada, że @p list wskazuje na poprawną listę.
 * @param[in] list   – wskaźnik na listę.
 * @return Wartość @p true jeśli lista jest pusta,
 * a @p false w przeciwnym przypadku.
 */
static inline bool list_is_empty(list_t *list) {
    assert(list);
    return list->left_guard->right == list->right_guard;
}

/** @brief Sprawdza czy wartość znajduje się na liście.
 * Sprawdza, czy któryś element listy @p list zawiera wartość @p value.
 * Zakłada, że @p list wskazuje na poprawną listę.
 * @param[in] list   – wskaźnik na listę,
 * @param[in] value  - wskaźnik którego obecność na liście jest sprawdzana.
 * @return Wartość @p true jeśli lista jest pusta,
 * a @p false w przeciwnym przypadku.
 */
bool list_in(list_t *list, void *value);

/** @brief Tworzy strukturę listy przechowującej wskaźniki.
 * Alokuje pamięć na nową strukturę listy przechowującej wskaźniki.
 * Inicjuje tę strukturę tak, aby reprezentowała pustą listę.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
list_t* list_new();

/** @brief Usuwa wszystkie elementy z listy.
 * Przywraca stan początkowy listy @p list, czyli usuwa wszystkie elementy spomiędzy
 * strażników oraz zwalnia ich pamięć. Jeśli @p list jest NULL, wówczas nie robi nic.
 * @param[in, out] list   – wskaźnik na listę.
 */
void list_clear(list_t *list);

/** @brief Usuwa listę wraz z elementami.
 * Usuwa wszystkie struktury listy @p list oraz zwalnia pamięć.
 * Jeśli @p list jest NULL, wówczas nie robi nic.
 * @param[in] list   – wskaźnik na listę.
 */
void list_delete(list_t *list);

/** @brief Daje wartość z początku listy.
 * Daje wartość przechowywaną przez pierwszy element listy.
 * Zakłada że lista @p list jest poprawna i niepusta.
 * @param[in] list   – wskaźnik na listę.
 * @return Wskaźnik będący pierwszą wartością na liście.
 */
void* list_first(list_t *list);

/** @brief Daje wartość z końca listy.
 * Daje wartość przechowywaną przez ostatni element listy.
 * Zakłada że lista @p list jest poprawna i niepusta.
 * @param[in] list   – wskaźnik na listę.
 * @return Wskaźnik będący ostatnią wartością na liście.
 */
void* list_last(list_t *list);

/** @brief Dodaje wskaźnik @p value na koniec listy.
 * Dodaje na koniec listy nowy element i ustawia jego wartość na @p value.
 * Zakłada że lista @p list jest poprawna.
 * @param[in, out] list  – wskaźnik na listę,
 * @param[in] value      - wskaźnik do dodania jako nowa wartość na listę.
 * * @return Wartość @p true jeśli operacja zakończyła się powodzeniem,
 * a @p false jeśli wystąpił błąd alokacji pamięci i lista nie uległa zmianie.
 */
bool list_append(list_t *list, void *value);

/** @brief Dodaje wskaźnik @p value na początek listy.
 * Dodaje na początek listy nowy element i ustawia jego wartość na @p value.
 * Zakłada że lista @p list jest poprawna.
 * @param[in, out] list  – wskaźnik na listę,
 * @param[in] value      - wskaźnik do dodania jako nowa wartość na listę.
 * @return Wartość @p true jeśli operacja zakończyła się powodzeniem,
 * a @p false jeśli wystąpił błąd alokacji pamięci i lista nie uległa zmianie.
 */
bool list_prepend(list_t *list, void *value);

/** @brief Usuwa wskaźnik z końca listy.
 * Usuwa z listy element zawierający ostatnią wartość.
 * Zakłada że lista @p list jest poprawna i niepusta.
 * @param[in, out] list  – wskaźnik na listę.
 */
void list_remove_last(list_t *list);

/** @brief Usuwa wskaźnik z początku listy.
 * Usuwa z listy element zawierający pierwszą wartość.
 * Zakłada że lista @p list jest poprawna i niepusta.
 * @param[in, out] list  – wskaźnik na listę.
 */
void list_remove_first(list_t *list);

/** @brief Daje wartość z początku listy i usuwa ją z listy.
 * Daje wartość przechowywaną przez pierwszy element listy,
 * a następnie usuwa ten element z listy.
 * Zakłada że lista @p list jest poprawna i niepusta.
 * @param[in, out] list   – wskaźnik na listę.
 * @return Wskaźnik będący pierwszą wartością na liście.
 */
void* list_pop_first(list_t *list);

/** @brief Daje wartość z końca listy i usuwa ją z listy.
 * Daje wartość przechowywaną przez ostatni element listy,
 * a następnie usuwa ten element z listy.
 * Zakłada że lista @p list jest poprawna i niepusta.
 * @param[in, out] list   – wskaźnik na listę
 * @return Wskaźnik będący ostatnią wartością na liście.
 */
void* list_pop_last(list_t *list);

/** @brief Woła iteratywnie funkcję z argumentami (@p another_param, kolejna wartość na liście).
 * Woła funkcję @p fun z kolejnymi parami argumentów: (@p another_param, wartość na liście @p list)
 * dla wszystkich kolejnych wartości listy od jej początku do końca.
 * Następnie usuwa listę i zwalnia całą zajmowaną przez nią pamięć.
 * Zakłada że lista @p list oraz funkcja @p fun są poprawne.
 * @param[in]      fun            - funkcja przyjmująca dwa wskaźniki void*,
 * @param[in, out] list           - wskaźnik na listę,
 * @param[in, out] another_param  - wskaźnik używany jako pierwszy argument wywołania
 *                                  funkcji @p fun.
 */
void list_iter_kamikaze(void (*fun)(void*, void*), list_t *list, void *another_param);


#endif //GAMMA_LIST_H
