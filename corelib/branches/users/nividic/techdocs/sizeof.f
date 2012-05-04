
      PROGRAM SIZEOF
      INTEGER J, K

      K = 1
      J = 1

10    IF (K > 0) THEN
        K = K + K
        J = J + 1
        GOTO 10
      END IF

      PRINT *, J / 8

      END

