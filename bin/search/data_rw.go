/**
 * Copyright (c) 2011 ~ 2014 Deepin, Inc.
 *               2013 ~ 2014 jouyouyun
 *
 * Author:      jouyouyun <jouyouwen717@gmail.com>
 * Maintainer:  jouyouyun <jouyouwen717@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 **/

package main

import (
	"bytes"
	"encoding/gob"
	"io/ioutil"
	"os"
	dutils "pkg.linuxdeepin.com/lib/utils"
)

func writeDatasToFile(datas interface{}, filename string) {
	if datas == nil {
		Logger.Warning("writeDatasToFile args error")
		return
	}

	var w bytes.Buffer
	enc := gob.NewEncoder(&w)
	if err := enc.Encode(datas); err != nil {
		Logger.Warning("Gob Encode Datas Failed:", err)
		return
	}

	fp, err := os.Create(filename)
	if err != nil {
		Logger.Warning("Open '%s' failed:", err)
		return
	}
	defer fp.Close()

	fp.WriteString(w.String())
	fp.Sync()
}

func readDatasFromFile(datas interface{}, filename string) bool {
	if !dutils.IsFileExist(filename) || datas == nil {
		Logger.Warning("readDatasFromFile args error")
		return false
	}

	contents, err := ioutil.ReadFile(filename)
	if err != nil {
		Logger.Warning("ReadFile '%s' failed:", err)
		return false
	}

	r := bytes.NewBuffer(contents)
	dec := gob.NewDecoder(r)
	if err = dec.Decode(datas); err != nil {
		Logger.Warning("Decode Datas Failed:", err)
		return false
	}

	return true
}
