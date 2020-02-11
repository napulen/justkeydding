
class Observations(object):
    def __init__(self):
        self.slices = []

    def append(self, l):
        try:
            _ = iter(l)
        except TypeError:
            print(l, 'is not iterable')
        self.slices.append(l)

    def __iter__(self):
        for idx, slic in enumerate(self.slices):
            for elem in slic:
                yield elem, idx

    @staticmethod
    def from_tuples(l):
        slices = Observations()
        max_index = max(l)[1]
        [slices.append([]) for _ in range(max_index + 1)]
        for elem, idx in l:
            slices.slices[idx].append(elem)
        return slices
