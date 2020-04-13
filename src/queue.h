/** @file
 * Interfejs kolejki przechowującej wskaźniki jako wartości.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 27.03.2020
 */

#ifndef GAMMA_QUEUE_H
#define GAMMA_QUEUE_H

#include "list.h"

/**
 * Typ kolejki przechowującej wskaźniki jako wartości.
 */
typedef list_t queue_t;

/** @brief Sprawdza czy kolejka jest pusta.
 * Sprawdza, czy na kolejce nie znajdują się żadne elementy.
 * Zakłada, że @p queue wskazuje na poprawną kolejkę.
 * @param[in] queue   – wskaźnik na kolejkę.
 * @return Wartość @p true jeśli kolejka jest pusta,
 * a @p false w przeciwnym przypadku.
 */
static inline bool queue_is_empty(queue_t *queue) {
    return list_is_empty(queue);
}

/** @brief Tworzy strukturę kolejki przechowującej wskaźniki.
 * Alokuje pamięć na nową strukturę kolejki przechowującej wskaźniki.
 * Inicjuje tę strukturę tak, aby reprezentowała pustą kolejkę.
 * @return Wskaźnik na utworzoną strukturę
 * lub NULL, gdy nie udało się zaalokować pamięci.
 */
static inline queue_t* queue_new() {
    return list_new();
}

/** @brief Usuwa kolejkę wraz z elementami.
 * Usuwa wszystkie struktury kolejki @p queue oraz zwalnia pamięć.
 * Jeśli @p queue jest NULL, wówczas nie robi nic.
 * @param[in] queue   – wskaźnik na kolejkę.
 */
static inline void queue_delete(queue_t *queue) {
    return list_delete(queue);
}

/** @brief Daje wartość z początku kolejki.
 * Daje wartość przechowywaną przez pierwszy element kolejki.
 * Zakłada że kolejka @p queue jest poprawna i niepusta.
 * @param[in] queue   – wskaźnik na kolejkę.
 * @return Wskaźnik będący pierwszą wartością na kolejce.
 */
static inline void* queue_peek(queue_t *queue) {
    return list_first(queue);
}

/** @brief Daje wartość z końca kolejki.
 * Daje wartość przechowywaną przez ostatni element kolejki.
 * Zakłada że kolejka @p queue jest poprawna i niepusta.
 * @param[in] queue   – wskaźnik na kolejkę.
 * @return Wskaźnik będący ostatnią wartością na kolejce.
 */
static inline void* queue_peek_last(queue_t *queue) {
    return list_last(queue);
}

/** @brief Wrzuca wskaźnik @p value na koniec kolejki.
 * Dodaje na koniec kolejki nowy element i ustawia jego wartość
 * na @p value. Zakłada że kolejka @p queue jest poprawna.
 * @param[in,out] queue  – wskaźnik na kolejkę,
 * @param[in] value      - wskaźnik do wrzucenia na kolejkę.
 */
static inline void queue_push(queue_t *queue, void* value) {
    list_append(queue, value);
}

/** @brief Wrzuca wskaźnik @p value na początek kolejki.
 * Dodaje na początek kolejki nowy element i ustawia jego wartość
 * na @p value. Zakłada że kolejka @p queue jest poprawna.
 * @param[in, out] queue   – wskaźnik na kolejkę,
 * @param[in] value        - wskaźnik do wrzucenia na kolejkę.
 */
static inline void queue_push_beg(queue_t *queue, void* value) {
    list_prepend(queue, value);
}

/** @brief Daje wartość z początku kolejki i usuwa ją z kolejki.
 * Daje wartość przechowywaną przez pierwszy element kolejki,
 * a następnie usuwa ten element z kolejki.
 * Zakłada że kolejka @p queue jest poprawna i niepusta.
 * @param[in, out] queue – wskaźnik na kolejkę.
 * @return Wskaźnik będący pierwszą wartością na kolejce.
 */
static inline void* queue_pop(queue_t *queue) {
    return list_pop_first(queue);
}

/** @brief Daje wartość z końca kolejki i usuwa ją z kolejki.
 * Daje wartość przechowywaną przez ostatni element kolejki,
 * a następnie usuwa ten element z kolejki.
 * Zakłada że kolejka @p queue jest poprawna i niepusta.
 * @param[in, out] queue   – wskaźnik na kolejkę.
 * @return Wskaźnik będący ostatnią wartością na kolejce.
 */
static inline void* queue_pop_last(queue_t *queue) {
    return list_pop_last(queue);
}

#endif //GAMMA_QUEUE_H
