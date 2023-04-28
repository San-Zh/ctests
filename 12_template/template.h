template <typename Tp>
struct stuOp;

template <>
struct stuOp<double> {
    int operator()() { return sizeof(double); }
};

template <>
struct stuOp<float> {
    int operator()() { return sizeof(float); }
};

enum DimEnum { x, y, z, t };

template <DimEnum>
class boundary {
    virtual int dim() const = 0;
};

template <>
class boundary<x> {
  public:
    int dim() { return x; }
};

template <>
class boundary<y> {
  public:
    int dim() { return y + 1; }
};

template <>
class boundary<z> {
  public:
    int dim() { return z * 2; }
};

template <>
class boundary<t> {
  public:
    int dim() { return t * t; }
};
