#ifndef SIREBASE_MD5SUM_H
#define SIREBASE_MD5SUM_H

#include "sireglobal.h"

#include "ThirdParty/md5.h"

SIRE_BEGIN_HEADER

class QFile;
class QString;
class QByteArray;

namespace SireBase
{

/**
 * These functions are used to simplify the generation of md5sums of 
 * buffers and files
 *
 * @author Christopher Woods
 */

class SIREBASE_EXPORT MD5Sum
{
public:
  /** Construct a null MD5Sum */
  MD5Sum();
  /** Construct an MD5Sum from a buffer */
  MD5Sum(const QByteArray &buffer);
  /**
   * Construct an MD5Sum from a const char* buffer.
   * @param buffer The buffer containing the data.
   * @param sz This size of the buffer. Don't lie otherwise it will crash!
   * @return A constructed, valid MD5Sum object. 
   */
  MD5Sum(const char *buffer, unsigned int sz);
  /** Construct the MD5Sum for a file */
  MD5Sum(const QString &file);
  /** Copy constructor */
  MD5Sum(const MD5Sum &sum);
  
  ~MD5Sum();
  
  /** Assignment operator */
  const MD5Sum& operator=(const MD5Sum &other);
  
  /** Equality operators */
  bool operator==(const MD5Sum &other) const;
  bool operator!=(const MD5Sum &other) const;
  
  /** Return a pointer to the digest array (16 element array) */
  const md5_byte_t* digest() const;
  
  /** Return a string representation of the digest */
  QString toString() const;

private:
  /**
   * Generate the md5sum digest.
   * @param buffer The buffer
   * @param sz The size of the buffer
   */
  void generate(const char *buffer, unsigned int sz);
  /** Pointer to the storage of the md5 digest */
  md5_byte_t* dgst;
};

}

SIRE_END_HEADER

#endif
