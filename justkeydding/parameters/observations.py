
class Observations(object):
    def __init__(self, slices=None):
        self.slices = []
        if slices:
            for l in slices:
                self.append(l)

    def append(self, l):
        try:
            _ = iter(l)
        except TypeError:
            print(l, 'is not iterable')
        for pc in l:
            if pc not in list(range(12)):
                raise TypeError(pc, "is not suported")
        self.slices.append(l)

    def __iter__(self):
        for _, slic in enumerate(self.slices):
            for elem in slic:
                yield elem
    
    def __getitem__(self, key):
        l = [elem for slic in self.slices for elem in slic]
        return l[key]
    
    def __len__(self):
        l = [elem for slic in self.slices for elem in slic]
        return len(l)
    
    def slice_indexes(self):
        for idx, slic in enumerate(self.slices):
            for _ in slic:
                yield idx
    
    def tuples(self):
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
