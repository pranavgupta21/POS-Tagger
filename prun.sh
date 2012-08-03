#!/bin/bash

make pos_tagger_clean
make pos_tagger_install
./pos_tagger < sample_text
