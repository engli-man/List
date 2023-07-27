#include <exception>
#include <iostream>
#include <memory>

struct BaseNode {
  BaseNode* next = nullptr;
  BaseNode* prev = nullptr;
};

template <typename T, typename Allocator = std::allocator<T>>
struct Node : BaseNode {
  T value;
  Node() = default;
  Node(const T& value) : value(value) {}
};

template <typename T, typename Allocator = std::allocator<T>>
class List {
 public:
  using allocator_type = Allocator;
  using allocator_type_traits = std::allocator_traits<Allocator>;
  using value_type = std::remove_cv_t<T>;
  using alloc_node =
      typename allocator_type_traits::template rebind_alloc<Node<T>>;
  using alloc_node_traits =
      typename allocator_type_traits::template rebind_traits<Node<T>>;
  template <bool IsConst, bool IsReversed>
  class Iterator;
  using iterator = Iterator<false, false>;
  using const_iterator = Iterator<true, false>;
  using reverse_iterator = Iterator<false, true>;
  using const_reverse_iterator = Iterator<true, true>;

  List() = default;

  ~List() {
    while (!empty()) {
      pop_back();
    }
  }

  List(size_t count, const T& value, const Allocator& alloc = Allocator())
      : alloc_(alloc) {
    try {
      for (size_t i = 0; i < count; ++i) {
        push_back(value);
      }
    } catch (...) {
      while (!empty()) {
        pop_back();
      }
    }
  }

  explicit List(size_t count, const Allocator& alloc = Allocator())
      : alloc_(alloc) {
    Node<T>* node = nullptr;
    try {
      for (size_t i = 0; i < count; ++i) {
        node = alloc_node_traits::allocate(alloc_, 1);
        alloc_node_traits::construct(alloc_, node);
        if (empty()) {
          end_node_.next = node;
          node->prev = &end_node_;
        } else {
          node->prev = end_node_.prev;
          node->prev->next = node;
        }
        end_node_.prev = node;
        node->next = &end_node_;
        count_++;
      }
    } catch (...) {
      alloc_node_traits::deallocate(alloc_, node, 1);
      while (!empty()) {
        pop_back();
      }
      throw;
    }
  }

  List(std::initializer_list<T> init, const Allocator& alloc = Allocator())
      : alloc_(alloc) {
    for (auto& itt : init) {
      push_back(itt);
    }
  }

  List(const List& other) {
    alloc_ =
        alloc_node_traits::select_on_container_copy_construction(other.alloc_);
    Node<T>* node = nullptr;
    for (auto& itt : other) {
      try {
        node = alloc_node_traits::allocate(alloc_, 1);
        alloc_node_traits::construct(alloc_, node, itt);
        if (empty()) {
          end_node_.next = node;
          node->prev = &end_node_;
        } else {
          node->prev = end_node_.prev;
          node->prev->next = node;
        }
        end_node_.prev = node;
        node->next = &end_node_;
        count_++;
      } catch (...) {
        alloc_node_traits::deallocate(alloc_, node, 1);
        while (!empty()) {
          pop_back();
        }
        throw;
      }
    }
  }

  explicit List(const List& other, const Allocator& alloc) : alloc_(alloc) {
    alloc_ =
        alloc_node_traits::select_on_container_copy_construction(other.alloc_);
    Node<T>* node = nullptr;
    for (auto& itt : other) {
      try {
        node = alloc_node_traits::allocate(alloc_, 1);
        alloc_node_traits::construct(alloc_, node, itt);
        if (empty()) {
          end_node_.next = node;
          node->prev = &end_node_;
        } else {
          node->prev = end_node_.prev;
          node->prev->next = node;
        }
        end_node_.prev = node;
        node->next = &end_node_;
        count_++;
      } catch (...) {
        alloc_node_traits::deallocate(alloc_, node, 1);
        while (!empty()) {
          pop_back();
        }
        throw;
      }
    }
  }

  List& operator=(const List& other) {
    List<T, Allocator> temp(other, other.alloc_);
    if (alloc_node_traits::propagate_on_container_copy_assignment::value &&
        alloc_ != other.alloc_) {
      alloc_ = other.alloc_;
    }
    std::swap(count_, temp.count_);
    std::swap(end_node_, temp.end_node_);

    return *this;
  }

  iterator begin() { return iterator(end_node_.next); }

  iterator end() { return iterator(&end_node_); }

  const_iterator begin() const {
    return const_iterator(const_cast<BaseNode*>(end_node_.next));
  }
  const_iterator end() const {
    return const_iterator((const_cast<BaseNode*>(&end_node_)));
  }

  const_iterator cbegin() const { return const_iterator(end_node_.next); }
  const_iterator cend() const { return const_iterator(&end_node_); }

  reverse_iterator rbegin() { return reverse_iterator(&end_node_); }
  reverse_iterator rend() { return reverse_iterator(end_node_.prev); }

  reverse_iterator rbegin() const {
    return reverse_iterator(const_cast<BaseNode*>(&end_node_));
  }
  reverse_iterator rend() const { return reverse_iterator(end_node_.prev); }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(&end_node_);
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(end_node_.prev);
  }

  bool empty() const noexcept { return count_ == 0; }
  size_t size() const noexcept { return count_; }

  T& front() { return *(begin()); }
  const T& front() const { return *(cbegin()); }

  T& back() {
    auto back_end = end();
    return *(--back_end);
  }
  const T& back() const {
    auto end = cend();
    return *(--end);
  }

  void push_back(const T& value) {
    try {
      Node<T>* node = alloc_node_traits::allocate(alloc_, 1);
      alloc_node_traits::construct(alloc_, node, value);
      if (empty()) {
        end_node_.next = node;
        node->prev = &end_node_;
      } else {
        node->prev = end_node_.prev;
        node->prev->next = node;
      }
      end_node_.prev = node;
      node->next = &end_node_;
      count_++;
    } catch (...) {
      pop_back();
      throw;
    }
  }

  void push_back(T&& value) {
    try {
      Node<T>* node = alloc_node_traits::allocate(alloc_, 1);
      alloc_node_traits::construct(alloc_, node, std::move(value));
      if (empty()) {
        end_node_.next = node;
        node->prev = &end_node_;
      } else {
        node->prev = end_node_.prev;
        node->prev->next = node;
      }
      end_node_.prev = node;
      node->next = &end_node_;
      count_++;
    } catch (...) {
      pop_back();
      throw;
    }
  }

  void pop_back() {
    Node<T>* temp = static_cast<Node<T>*>(end_node_.prev);
    end_node_.prev = temp->prev;
    temp->prev->next = &end_node_;
    alloc_node_traits::destroy(alloc_, temp);
    alloc_node_traits::deallocate(alloc_, temp, 1);
    count_--;
  }

  void push_front(const T& value) {
    try {
      Node<T>* node = alloc_node_traits::allocate(alloc_, 1);
      alloc_node_traits::construct(alloc_, node, value);
      if (empty()) {
        end_node_.prev = node;
        node->next = &end_node_;
      } else {
        node->prev = &end_node_;
        end_node_.next->prev = node;
      }
      node->next = end_node_.next;
      end_node_.next = node;
      count_++;
    } catch (...) {
      pop_front();
      throw;
    }
  }

  void push_front(T&& value) {
    try {
      Node<T>* node = alloc_node_traits::allocate(alloc_, 1);
      alloc_node_traits::construct(alloc_, node, std::move(value));
      if (empty()) {
        end_node_.prev = node;
        node->next = &end_node_;
      } else {
        node->prev = &end_node_;
        end_node_.next->prev = node;
      }
      node->next = end_node_.next;
      end_node_.next = node;
      count_++;
    } catch (...) {
      pop_front();
      throw;
    }
  }

  void pop_front() {
    Node<T>* temp = static_cast<Node<T>*>(end_node_.next);
    end_node_.next = temp->next;
    temp->next->prev = &end_node_;
    alloc_node_traits::destroy(alloc_, temp);
    alloc_node_traits::deallocate(alloc_, temp, 1);
    count_--;
  }

  template <bool IsConst, bool IsReversed>
  class Iterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_cv_t<T>;
    using m_type = std::conditional_t<IsConst, const T, T>;
    using pointer = m_type*;
    using iterator_category = std::bidirectional_iterator_tag;
    using reference = m_type&;
    using node_type = std::conditional_t<IsConst, const Node<T>, Node<T>>;
    using node_pointer = node_type*;

    Iterator(BaseNode* ptr) : ptr_(static_cast<Node<T>*>(ptr)) {}

    Iterator<IsConst, IsReversed>& operator++() {
      if (IsReversed) {
        ptr_ = static_cast<Node<T>*>(ptr_->prev);
        return *this;
      }
      ptr_ = static_cast<Node<T>*>(ptr_->next);
      return *this;
    }
    Iterator<IsConst, IsReversed>& operator--() {
      if (IsReversed) {
        ptr_ = static_cast<Node<T>*>(ptr_->next);
        return *this;
      }
      ptr_ = static_cast<Node<T>*>(ptr_->prev);
      return *this;
    }
    Iterator<IsConst, IsReversed> operator++(int) {
      if (IsReversed) {
        auto temp = *this;
        --*this;
        return temp;
      }
      auto temp = *this;
      ++*this;
      return temp;
    }
    Iterator<IsConst, IsReversed> operator--(int) {
      if (IsReversed) {
        auto temp = *this;
        ++*this;
        return temp;
      }
      auto temp = *this;
      --*this;
      return temp;
    }

    bool operator==(const Iterator<IsConst, IsReversed>& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const Iterator<IsConst, IsReversed>& other) const {
      return !(*this == other);
    }
    reference operator*() const { return ptr_->value; }

    pointer operator->() const { return &(ptr_->value); }

   private:
    Node<T>* ptr_;
  };

  allocator_type get_allocator() { return alloc_; }

 private:
  BaseNode end_node_;
  size_t count_ = 0;
  alloc_node alloc_;
};