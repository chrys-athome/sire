

/** Construct from another set of ForceFields */
ForceFields::ForceFields(const ForceFieldsBase &base)
            : ForceFieldsBase(base), 
              ffields(base.forceFields())
{}
