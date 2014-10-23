#pragma once

template <typename FST, typename SND>
class Pair {
	public:
		Pair() {
		}
	
		Pair(const FST fst, const SND snd) {
			this->fst = fst;
			this->snd = snd;
		}
		
		FST fst;
		SND snd;
};
