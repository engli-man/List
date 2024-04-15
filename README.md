# List

Необходимо реализовать шаблонный класс List\<T, Allocator\> аналог std::list из stl.
Реализация основана на нодах (класс Node) которые содержат указатели налево и направо.

List должен уметь в:
* Базовую функциональность + итераторы
* Поддержку аллокаторов
* Быть exception-safety

## Методы:

### using'и:
 * value_type
 * allocator_type
 * iterator
### Constructors:
 * List()
 * List(size_t count, const T& value = T(), const Allocator& alloc = Allocator())
 * explicit List(size_t count, const Allocator& alloc = Allocator())
 * List(const list& other);
 * List(std::initializer_list\<T\> init, const Allocator& alloc = Allocator())
### Destrustor
### Iterator (с поддержкой константных):
 * begin(),cbegin()
 * end(), cend()
### operator=:
 * List& operator=(const List& other) (используется propagate_on_container_copy_assignment)
### element access methods:
 * T& front()
 * const T& front() const
 * T& back()
 * const T& back() const
### capacity:
 * bool empty()
 * size_t size()
### modifiers:
 * push_back(front)(const T&)
 * push_back(front)(T&&)
 * pop_back(front)();
