#ifndef miscclass_POINTER_HH_
#define miscclass_POINTER_HH_

#include <cstring>   // memcpy
#include <cstdlib>   // free

/**
   Automatische Zeigervariable, die einen Zeiger auf ein einzelnes Objekt
   verwaltet.
   @short Automatische Zeigervariable.
   @version 0.1
   @author Michael Saure
*/
template<class T>
class ptr
{
  T *m_cont;

public:
  /** Der Standardkonstruktor erzeugt einen Nullzeiger. */
  ptr() : m_cont( 0) {}

  /** Assign a pointer value. */
  ptr( T *cont) : m_cont( cont) {}

  /** Lˆschen des Inhalts. */
  ~ptr( ) {
    if (0 != m_cont)
      delete m_cont;
  }

  operator T*() {
    return m_cont;
  }

  /** Zuweisung eines Zeigers. */
  ptr &operator=( T *c) {
    if (0 != m_cont)
      delete m_cont;
    m_cont = c;
    return *this;
  }

  /** Zugriff auf den Inhalt. */
  T *p() { return m_cont; } 
  T &operator*() { return *m_cont; }
  T *operator->() { return m_cont; }
};


/**
   Automatische Zeigervariable, die einen Zeiger auf ein dynamisches Array
   verwaltet.
   @short Automatische Arrayvariable.
   @version 0.1
   @author Michael Saure
*/
template<class T>
class array {
  T *m_cont;
  size_t m_size;

public:
  typedef T value_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;

  /**
   Zuweisung eines Zeigers. Die Daten, auf die c zeigt, werden nicht
   dupliziert.
  */
  explicit array( size_t s) {
    m_cont = new T[m_size=s];
  }

  /**
    'deep copy' eines array.
  */
  array( const array<T> &a) {
    memcpy( m_cont, a.m_cont, m_size);
    m_size = a.m_size;
  }

  ~array() { delete [] m_cont; }


  /** Zuweisung eines array. Vorher durch das Zielobjekt referenzierte Daten
      werden gelˆscht.
  */
  array &operator=( const array<T> &a) {
    if (this != &a) {
      if (0 != m_cont) {
	delete [] m_cont;
        m_cont = new T[m_size=a.m_size];
	m_cont = memcpy( m_cont, a.m_cont, sizeof( a.m_cont));
      }
    }
  }

  /** Umwandlung in den Inhaltstypen. */
  T *p() { return m_cont; }

  const T *p() const { return m_cont; }

  operator T*() { return m_cont; }

  /** Schreib- und Lesezugriff auf Elemente des Arrays. */
  reference operator[]( int i) { return cont[i]; }

  /** Ausschlie†lich lesender Zugriff auf Elemente des Arrays. Es werden
      Kopien der Elemente zurÅckgeliefert.
  */
  const_reference operator[]( int i) const { return cont[i]; }
};

typedef array<char> char_ptr;

template<class T> class c_ptr
{
  T *cont;

public:
  /** Der Standardkonstruktor erzeugt einen Nullzeiger. */
  c_ptr() { cont = 0; }

  /** Zuweisung eines Zeigers. */
  c_ptr( T *c) { cont = c; }

  /** Lˆschen des Inhalts. */
  ~c_ptr( ) {
    if (0 != cont)
      free( cont);
  }

  operator T*() { return cont; }

  /** Zuweisung eines Zeigers. */
  c_ptr &operator=( T *c) {
    if (0 != cont)
      free( cont);
    cont = c;
    return *this;
  }

  /** Zugriff auf den Inhalt. */
  T *p() { return cont; } 
};



/**
   Ein Zeiger mit einem Tag zur Verwaltung von ReferenzzÑhlern.
   @version 0.1
   @author Michael Saure
*/
template<class T> struct tagged_ptr
{
  int rc;
  T *ptr;

  tagged_ptr() { rc = 0; ptr = 0; }
  ~tagged_ptr();
  void incrRef() { ++rc; }
  void decrRef() { --rc; }
};

/**
   Automatische Zeigervariable, die shared copies eines Zeigers auf ein
   einzelnes Objekt verwaltet.
   @short Automatische Zeigervariable mit ReferenzzÑhler.
   @version 0.1
   @author Michael Saure
*/
template<class T> class s_ptr
{
  /** Zeiger auf die verwalteten Daten. */
  tagged_ptr<T> *cont;

public:
  s_ptr() { rc = 0; cont = 0; }

  s_ptr( T *c) { rc = 1; cont = c; }

  ~s_ptr() {
    if (0 != cont && rc <= 1)
      delete cont;
    --rc;
  }
};

#endif /*miscclass_POINTER_HH_*/
