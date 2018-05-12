class Stream:
    def __init__(self, i):
        self.iter = iter(i)

    def __next__(self):
        return next(self.iter)

    def __iter__(self):
        return self

    def to_list(self):
        return list(self)

    def select(self, func):
        class SelectStream(Stream):
            def __next__(self):
                return func(next(self.iter))

        return SelectStream(self)

    def where(self, func):
        class WhereStream(Stream):
            def __next__(self):
                a = next(self.iter)
                while not func(a):
                    a = next(self.iter)
                return a

        return WhereStream(self)

    def take(self, n):
        class TakeStream(Stream):
            def __init__(self, i, n):
                super().__init__(i)
                self.n = n

            def __next__(self):
                if self.n > 0:
                    self.n -= 1
                    return next(self.iter)
                else:
                    raise StopIteration

        return TakeStream(self, n)

    def order_by(self, key=None, reverse=False):
        class OrderByStream(Stream):
            def __init__(self, i):
                super().__init__(sorted(i, key=key, reverse=reverse))

        return OrderByStream(self)

    def group_by(self, key=(lambda x: x)):
        class GroupByStream(Stream):
            def __init__(self, i, key):
                super().__init__(sorted(i, key=key))
                self.key_func = key
                self.tgtkey = self.currkey = self.currvalue = object()

            def __iter__(self):
                return self

            def __next__(self):
                while self.currkey == self.tgtkey:
                    self.currvalue = self.iter.__next__()
                    self.currkey = self.key_func(self.currvalue)
                self.tgtkey = self.currkey
                return self.currkey, Stream(self._group(self.tgtkey))

            def _group(self, key):
                while self.currkey == key:
                    yield self.currvalue
                    try:
                        self.currvalue = self.iter.__next__()
                    except StopIteration:
                        return
                    self.currkey = self.key_func(self.currvalue)

        return GroupByStream(self, key)

    def count(self):
        if hasattr(self.iter, '__len__'):
            return len(self.iter)
        iterated = list(self.iter)
        self.iter = iter(iterated)
        return len(iterated)

    def flatten(self):
        class FlattenStream(Stream):
            def __init__(self, i):
                super().__init__(i)
                self.currlist = next(i)

            def __next__(self):
                try:
                    return next(self.currlist)
                except StopIteration:
                    self.currlist = next(self.iter)
                    return next(self.currlist)

        return FlattenStream(self)


def fibb(a, b):
    while True:
        a, b = b, a + b
        yield a


# Task 1
print(Stream(fibb(0, 1)).where(lambda x: x % 3 == 0).select(lambda x: x**2 if x % 2 == 0 else x).take(5).to_list())

# Task 2
with open('linq.py') as file:
    print(Stream(file).select(lambda x: iter([i.strip() for i in x.split(' ')])).flatten().select(lambda x: ''.join(x)).where(lambda x: x).group_by().select(lambda x: (x[1].count(), x[0])).order_by(reverse=True).to_list())
