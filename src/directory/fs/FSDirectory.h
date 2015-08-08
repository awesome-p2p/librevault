/* Copyright (C) 2015 Alexander Shishenko <GamePad64@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include "../../pch.h"
#include "../Abstract.h"

#include "../Key.h"
#include "Index.h"
#include "EncStorage.h"
#include "OpenStorage.h"
#include "Indexer.h"
#include "AutoIndexer.h"

namespace librevault {

class FSProvider;
class FSDirectory {
public:
	FSDirectory(ptree dir_options, Session& session, FSProvider& provider);
	virtual ~FSDirectory();

	const ptree& dir_options() const {return dir_options_;}

	const Key& key() const {return key_;}
	const blob& hash() const {return key().get_Hash();}

	const fs::path& open_path() const {return open_path_;}
	const fs::path& block_path() const {return block_path_;}
	const fs::path& db_path() const {return db_path_;}
	const fs::path& asm_path() const {return asm_path_;}

	// Components
	std::unique_ptr<Index> index;
	std::unique_ptr<EncStorage> enc_storage;
	std::unique_ptr<OpenStorage> open_storage;
	std::unique_ptr<Indexer> indexer;
	std::unique_ptr<AutoIndexer> auto_indexer;

private:
	std::shared_ptr<spdlog::logger> log_;

	// Directory options
	ptree dir_options_;

	// Key
	const Key key_;

	// Paths
	const fs::path open_path_, block_path_, db_path_, asm_path_;

	// Statistics
	uint64_t received_bytes_ = 0;
	uint64_t sent_bytes_ = 0;

	// Revision operations
	void handle_smeta(AbstractDirectory::SignedMeta smeta);

	void announce_revision(const blob& path_hmac, int64_t revision);	// When FSDirectory posts to others.
	void apply_revision();	// Apply revision information to this FSDirectory.
};

} /* namespace librevault */