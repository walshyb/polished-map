import { MenuItem, Icon, Menu, MenuHeader } from 'semantic-ui-react'
import { useState } from 'react';
import { useAppSelector, useAppDispatch } from '../../hooks';

export default function FileExplorer(
  { fileExplorerActive }: { fileExplorerActive: boolean }
) {
  const files = useAppSelector((state) => state.file.files);

  if (!fileExplorerActive) {
    return null;
  }

  return (
    <div>
      <Menu
        icon='labeled'
        vertical
        floated={true}
        style={{ display: 'flex', height: '100%', marginTop: 0 }}
      >
        <MenuHeader>
          File Explorer
        </MenuHeader>
        { files.map((file, index) => {
          return (
            <MenuItem
              key={index}
              name={file.name}
              active={false}
              onClick={() => {}}
            >
              {file.name}
            </MenuItem>
          );
        })}
      </Menu>
    </div>
  );
}
