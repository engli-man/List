# List

Необходимо реализовать шаблонный класс List\<T, Allocator\> аналог std::list из stl.

Реализация должна быть основана на нодах (класс Node) которые содержат указатели на лево и на право.

List должен уметь в:

* Базовую функциональность + итераторы
* Поддержку аллокаторов
* Быть exception-safety

## Методы:

### Using'и

* value_type
* allocator_type
* iterator (должен удовлетворять https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)

### Конструкторы

Реализованы следующие конструкторы:

* List()
* List(size_t count, const T& value = T(), const Allocator& alloc = Allocator())
* explicit List(size_t count, const Allocator& alloc = Allocator())
* List(const list& other);
* List(std::initializer_list\<T\> init, const Allocator& alloc = Allocator())

### Деструктор

### Iterators (с поддержкой константных)

* begin()
* end()
* cbegin()
* cend()

### operator=

* List& operator=(const List& other) (используется propagate_on_container_copy_assignment)


### element access methods

* T& front()
* const T& front() const
* T& back()
* const T& back() const


### Capacity

* bool empty()
* size_t size()

### Modifiers

* push_back(front)(const T&)
* push_back(front)(T&&)
* pop_back(front)();

### Поддержка аллокаторов

* Работа с памятью через аллокатор
* Конструирование и разрушение объектов через аллокатор
* Поддержка propagate_on_container_copy в соответствующих методах
* Использование rebind для аллоцирования и конструирования внутреннего класса Node

### Exception-safety

# List
